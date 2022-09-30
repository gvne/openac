#include "oac/dntp/client.h"
#include <spdlog/spdlog.h>
#include "oac/chrono/local_clock.h"

namespace oac {
namespace dntp {

// The DNTP client calls the server to update the time offset
// To avoid calling the server too often, if the received offset error
// (abs(received offset - mean offsets)) is bellow the standard deviation, we
// increase the time to next call
// However, in case of an abrupt change (clock resync for example), whole window
// should be overriden swiftly
// we update the period using p(i) = p(i-1) + sqrt(p(i-1)) / 6.5
// and pmin = 0.250 seconds. This way
// i    |  period  |  period sum
// 0    | 0.250    | 0
// 1    | 0.326    | 0.25
// 2    | 0.414    | 0.576
// 3    | 0.513    | 0.991
// 4    | 0.624    | 1.505
// 5    | 0.745    | 2.130
// 6    | 0.878    | 2.875
// 7    | 1.022    | 3.754
// 8    | 1.178    | 4.777
// 9    | 1.345    | 5.956
// a sliding window of size 10 represents 6seconds which is acceptable
std::chrono::milliseconds UpdatePeriod(std::chrono::milliseconds period) {
  auto inc_ms = sqrt(static_cast<double>(period.count())) / 6.5;
  std::chrono::milliseconds inc(static_cast<uint64_t>(std::round(inc_ms)));
  return period + inc;
}
const std::chrono::milliseconds kPeriodMin(250);
const std::chrono::milliseconds kPeriodMax(3000);
const uint8_t kSlidingWindowSize(10);

Client::Client(asio::io_context& context) :
  callback_([](Nanoseconds, Nanoseconds){}),
  context_(context),
  socket_(context),
  timer_(context, std::chrono::seconds(0)),
  request_datagram_(request_.length()),
  response_datagram_(response_.length()),
  period_(kPeriodMin),
  time_offsets_(kSlidingWindowSize),
  round_trip_delays_(kSlidingWindowSize),
  last_update_time_offset_(0) {}

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
    asio::buffer(response_datagram_),
    [&](const asio::error_code& cerr, std::size_t size) {
      std::error_code err;
      spdlog::trace("dntp - Received response");
      auto final_time = Now();
      if (cerr) {
        spdlog::warn("dntp - Could not receive server response: {}", cerr.message());
        ListenResponse();
        return;
      }
      
      Parse(response_datagram_.data(), size, response_, err);
      if (err) {
        spdlog::debug("dntp - Could not decode response");
        ListenResponse();
        return;
      };

      auto round_trip_delay = RoundTripDelay(response_, final_time);
      auto time_offset = TimeOffset(response_,final_time);

      auto time_offset_s = ToSeconds(time_offset);
      auto round_trip_delay_s = ToSeconds(round_trip_delay);

      auto offset_mean = time_offsets_.mean();
      auto offset_std = time_offsets_.standard_deviation();

      auto offset_err = std::abs(time_offset_s - offset_mean);
      if (offset_err < offset_std * 10) {
        // if the offset is in the standard deviation range we can reduce the frequency at which we query the server
        period_ = std::min(UpdatePeriod(period_), kPeriodMax);
      } else {
        // else, we reset the polling period to increase the number of received values
        spdlog::debug("dntp - Resetting frequency (err={:.4f}s, std={:.4f}s)", offset_err, offset_std);
        period_ = kPeriodMin;
      }

      time_offsets_.Push(time_offset_s);
      round_trip_delays_.Push(round_trip_delay_s);

      offset_mean = time_offsets_.mean();
      offset_err = std::abs(last_update_time_offset_ - offset_mean);
      if (offset_err > offset_std) {
        last_update_time_offset_ = offset_mean;
        auto rtd = round_trip_delays_.mean();
        asio::post(context_, [this, offset_mean, rtd](){
          callback_(FromSeconds(offset_mean), FromSeconds(rtd));
        });
      }

      ListenResponse();
    }
  );
}

void Client::SendRequest() {
  timer_.async_wait([&](const asio::error_code& cerr){
    std::error_code err;
    timer_.expires_at(timer_.expiry() + period_);
    spdlog::trace("dntp - Sending request");
    request_.field_originate_timestamp().value() = Now().value();
    
    Serialize(request_, request_datagram_.data(), request_datagram_.size(), err);
    if (err) {
      spdlog::debug("dntp - Could Serialize request");
      SendRequest();
      return;
    }
    
    socket_.send_to(asio::buffer(request_datagram_), server_addr_, 0, err);
    if (err) {
      spdlog::debug("dntp - Couldn't send request: {}", err.message());
    }
    SendRequest();
  });
}

Timestamp Client::Now() const {
  return FromTimePoint(chrono::LocalClock::now());
}

void Client::set_period(std::chrono::milliseconds period) {
  period_ = std::move(period);
}

std::chrono::milliseconds Client::period() const {
  return period_;
}

double Client::ToSeconds(const Nanoseconds& value) {
  return static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(value).count()) / 1e9;
}

Client::Nanoseconds Client::FromSeconds(double value) {
  return std::chrono::nanoseconds(static_cast<uint64_t>(value * 1e9));
}

}  // namespace dntp
}  // namespace oac
