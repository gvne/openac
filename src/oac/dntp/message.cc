#include "oac/dntp/message.h"

namespace oac {
namespace dntp {

void Parse(const uint8_t* data,
           std::size_t data_size,
           Message& message,
           std::error_code& err) {
  auto status = message.doRead(data, data_size);
  if (status == comms::ErrorStatus::Success) {
    return;
  }
  err = std::make_error_code(std::errc::protocol_error);
}

void Serialize(const Message& message,
               uint8_t* data,
               std::size_t data_size,
               std::error_code& err) {
  auto status = message.doWrite(data, data_size);
  if (status == comms::ErrorStatus::Success) {
    return;
  }
  err = std::make_error_code(std::errc::protocol_error);
}

bool IsEqual(const Timestamp& ts, const Timestamp& other) {
  return ts.field_second().value() == other.field_second().value() && \
    ts.field_fraction().value() == other.field_fraction().value();
}

TimePoint ToTimePoint(const Timestamp& ts) {
  uint64_t duration_ns = static_cast<double>(ts.field_fraction().value() * 1e9) / std::numeric_limits<uint32_t>::max();
  std::chrono::nanoseconds duration(duration_ns);
  
  return TimePoint(duration + std::chrono::seconds(ts.field_second().value()));
}

// See https://fr.wikipedia.org/wiki/Network_Time_Protocol
std::chrono::nanoseconds RoundTripDelay(const Message& msg,
                                        const Timestamp& final_timestamp) {
  auto t0 = ToTimePoint(msg.field_originate_timestamp());
  auto t1 = ToTimePoint(msg.field_receive_timestamp());
  auto t2 = ToTimePoint(msg.field_transmit_timestamp());
  auto t3 = ToTimePoint(final_timestamp);
  
  return (t3 - t0) - (t2 - t1);
}

std::chrono::nanoseconds TimeOffset(const Message& msg,
                                    const Timestamp& final_timestamp) {
  auto t0 = ToTimePoint(msg.field_originate_timestamp());
  auto t1 = ToTimePoint(msg.field_receive_timestamp());
  auto t2 = ToTimePoint(msg.field_transmit_timestamp());
  auto t3 = ToTimePoint(final_timestamp);
  
  return ((t1 - t0) + (t2 - t3)) / 2;
}

}  // namespace dntp
}  // namespace oac

