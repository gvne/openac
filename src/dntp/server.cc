#include "dntp/server.h"
#include <spdlog/spdlog.h>
#include "dntp/timestamp.h"

namespace oac {
namespace dntp {

Server::Server(asio::io_context& context) :
  context_(context),
  socket_(context) {}

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
    asio::buffer(message_.data(), message_.size()), origin_,
    [&](const asio::error_code& cerr, std::size_t size) {
      spdlog::trace("dntp - Received a request");
      auto receive_timestamp = Now().Pack();
      if (cerr) {
        spdlog::debug("dntp - Invalid message received {}", cerr.message());
        Run();
        return;
      }

      if (message_.version != message::kVersion) {
        spdlog::debug("dntp - Invalid message version received {}", message_.version);
        Run();
        return;
      }
      
      auto version = message_.version;
      auto originate_timestamp = message_.originate_timestamp;
      auto origin = origin_;
      asio::post(context_, [&, version, originate_timestamp, receive_timestamp, origin]() mutable {
        spdlog::trace("dntp - Sending response");
        Message response;
        response.version = version;
        response.originate_timestamp = originate_timestamp;
        response.receive_timestamp = receive_timestamp;
        response.transmit_timestamp = Now().Pack();
        
        std::error_code err;
        socket_.send_to(
          asio::buffer(response.data(), response.size()), origin, 0, err);
        if (err) {
          spdlog::debug("dntp - Could not reply: {}", err.message());
        }
      });
      Run();
    }
  );
}

Timestamp Server::Now() const {
  return Timestamp::Now<std::chrono::system_clock>();
}

asio::ip::udp::endpoint Server::endpoint() const {
  return socket_.local_endpoint();
}

}  // namespace dntp
}  // namespace oac
