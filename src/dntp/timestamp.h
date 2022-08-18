#ifndef OAC_DNTP_TIMESTAMP_H_
#define OAC_DNTP_TIMESTAMP_H_

#include <cstdint>
#include <chrono>

namespace oac {
namespace dntp {

struct Timestamp {
  template <class ReferenceClock>
  static Timestamp Now() {
    auto tp = ReferenceClock::now();
    Timestamp ts;

    // seconds
    auto duration = tp.time_since_epoch();
    auto duration_s = std::chrono::duration_cast<std::chrono::seconds>(duration);
    ts.seconds = duration_s.count();

    // fraction
    duration -= duration_s;
    auto duration_ns = \
      std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
    ts.fraction = \
      (duration_ns.count() * std::numeric_limits<uint32_t>::max()) / 1e9;
    
    return ts;
  }
  
  uint64_t Pack() const;
  static Timestamp Unpack(uint64_t value);
  
  using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
  TimePoint ToTimePoint() const;

  int32_t seconds;
  uint32_t fraction;
} __attribute__((packed));

}  // namespace dntp
}  // namespace oac

#endif  // OAC_DNTP_TIMESTAMP_H_
