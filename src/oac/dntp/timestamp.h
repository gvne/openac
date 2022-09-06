#ifndef OAC_DNTP_TIMESTAMP_H_
#define OAC_DNTP_TIMESTAMP_H_

#include <cstdint>
#include <chrono>

#include "astruct/astruct.h"

namespace oac {
namespace dntp {

/// NTP timestamp [as defined here](https://en.wikipedia.org/wiki/Network_Time_Protocol#Timestamps)
ASTRUCT(Timestamp,
  (int32_t, seconds),
  (uint32_t, fraction)
);

namespace timestamp {

/// Get the timestamp for a specific time point
/// \tparam Clock the std::chrono clock the time_point refers to
/// \param tp the time point
template <class Clock>
Timestamp FromTimePoint(std::chrono::time_point<Clock> tp) {
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

/// Get the current timestamp for a specific clock
/// \tparam Clock the std::chrono clock to obtain the current time from
template <class ReferenceClock = std::chrono::system_clock>
Timestamp Now() {
  return FromTimePoint(ReferenceClock::now());
}

/// Pack the timestamp into a 64 bits unsigned it
/// \param ts the timestamp
/// \return the packed value
uint64_t Pack(const Timestamp& ts);

/// Unpack the timestamp from a 64 bits unsigned it
/// \param value the packed value
/// \return the timestamp
Timestamp Unpack(uint64_t value);

using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

/// Get a time point from a timestamp
/// \param ts the dntp timestamp
/// \return the associated high resolution clock time point
TimePoint ToTimePoint(const Timestamp& ts);
}  // namespace timestamp

}  // namespace dntp
}  // namespace oac

#endif  // OAC_DNTP_TIMESTAMP_H_
