#include "dntp/client.h"
#include <spdlog/spdlog.h>
#include "dntp/timestamp.h"

namespace oac {
namespace dntp {

Client::Client(asio::io_context& context) :
  period_(3000),  // default period is 3 seconds
  callback_([](Nanoseconds, Nanoseconds){}),
  context_(context),
  socket_(context),
  timer_(context, std::chrono::seconds(0)) {}

void Client::Start(asio::ip::udp::endpoint server_addr,
                   UpdateCallback callback,
                   std::error_code &err) {
  callback_ = callback;
  socket_.open(asio::ip::udp::v4(), err);
  if (err) {
    spdlog::error("dntp - Socket error: {}", err.message());
    return;
  }

  socket_.bind(asio::ip::udp::endpoint(asio::ip::udp::v4(), 0), err);
  if (err) {
    spdlog::error("dntp - Bind error: {}", err.message());
    return;
  }

  server_addr_ = std::move(server_addr);

  ListenResponse();
  SendRequest();
}

void Client::ListenResponse() {
  socket_.async_receive(
    asio::buffer(&received_message_, sizeof(received_message_)),
    [&](const asio::error_code& cerr, std::size_t size) {
      spdlog::trace("dntp - Received response");
      auto final_time = Now().Pack();
      if (cerr) {
        spdlog::warn("dntp - Could not receive server response: {}", cerr.message());
        ListenResponse();
        return;
      }

      auto round_trip_delay = received_message_.RoundTripDelay(final_time);
      auto time_offset = received_message_.TimeOffset(final_time);
      asio::post(context_, [this, round_trip_delay, time_offset](){
        callback_(time_offset, round_trip_delay);
      });
      ListenResponse();
    }
  );
}

void Client::SendRequest() {
  timer_.async_wait([&](const asio::error_code& cerr){
    timer_.expires_at(timer_.expiry() + period_);
    spdlog::trace("dntp - Sending request");
    sent_message_.version = Message::kVersion;
    sent_message_.originate_timestamp = Now().Pack();
    std::error_code err;
    socket_.send_to(asio::buffer(&sent_message_, sizeof(Message)), server_addr_, 0, err);
    if (err) {
      spdlog::debug("dntp - Couldn't send request: {}", err.message());
    }
    SendRequest();
  });
}

Timestamp Client::Now() const {
  return Timestamp::Now<std::chrono::system_clock>();
}

void Client::set_period(std::chrono::milliseconds period) {
  period_ = std::move(period);
}

std::chrono::milliseconds Client::period() const {
  return period_;
}

}  // namespace dntp
}  // namespace oac

