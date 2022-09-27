#include "oac/dntp/utils.h"

namespace oac {
namespace dntp {

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
