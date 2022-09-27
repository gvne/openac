#include "oac/dntp/server.h"
#include <spdlog/spdlog.h>

#include "oac/chrono/local_clock.h"

namespace oac {
namespace dntp {

Server::Server(asio::io_context& context) :
  context_(context),
  socket_(context)
  {
    // We know that we will send and receive message of fixed length
    // We can initialize the datagrams
    Message msg;
    request_datagram_.resize(msg.length());
    response_datagram_.resize(msg.length());
  }

void Server::Start(std::error_code& err) {
  Start(0, err);
}

void Server::Start(uint16_t port, std::error_code& err) {
  socket_.open(asio::ip::udp::v4(), err);
  if (err) {
    return;
  }
  socket_.bind(
    asio::ip::udp::endpoint(asio::ip::udp::v4(), port), err);
  if (err) {
    return;
  }
  Run();
}

void Server::Run() {
  
  socket_.async_receive_from(
    asio::buffer(request_datagram_), origin_,
    [&](const asio::error_code& cerr, std::size_t size) {
      spdlog::trace("dntp - Received a request");
      auto receive_timestamp = Now();
      if (cerr) {
        spdlog::debug("dntp - Invalid message received {}", cerr.message());
        Run();
        return;
      }
      
      // Decode the message from the datagram
      auto request_datagram_iterator = request_datagram_.begin();
      if (request_.read(request_datagram_iterator, size) != comms::ErrorStatus::Success) {
        spdlog::debug("dntp - Could not decode received message");
        Run();
        return;
      };
    
      // Verify the version
      if (request_.field_version().value() != 1) {
        spdlog::debug("dntp - Invalid message version received {}", request_.field_version().value());
        Run();
        return;
      }
      
      Message response = request_;
      response.field_receive_timestamp().value() = receive_timestamp.value();
      auto origin = origin_;
      asio::post(context_, [&, response, origin]() mutable {
        spdlog::trace("dntp - Sending response");
        response.field_transmit_timestamp().value() = Now().value();
        
        // set response data in a datagram
        auto response_iterator = response_datagram_.begin();
        if (response.write(response_iterator, response_datagram_.size()) != comms::ErrorStatus::Success) {
          spdlog::debug("dntp - Could Serialize response");
          return;
        }
        
        std::error_code err;
        socket_.send_to(
          asio::buffer(response_datagram_), origin, 0, err);
        if (err) {
          spdlog::debug("dntp - Could not reply: {}", err.message());
        }
      });
      Run();
    }
  );
}

Timestamp Server::Now() const {
  return FromTimePoint(chrono::LocalClock::now());
}

asio::ip::udp::endpoint Server::endpoint() const {
  return socket_.local_endpoint();
}

}  // namespace dntp
}  // namespace oac
