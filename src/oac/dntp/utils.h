#ifndef OAC_DNTP_UTILS_H_
#define OAC_DNTP_UTILS_H_

#include "dntp_msg/field/Timestamp.h"
#include "dntp_msg/field/Message.h"

#include "oac/chrono/local_clock.h"

namespace oac {
namespace dntp {

using Timestamp = dntp_msg::field::Timestamp<>;
using Message = dntp_msg::field::Message<>;
using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

/// Get the timestamp for a specific time point
/// \tparam Clock the std::chrono clock the time_point refers to
/// \param tp the time point
template <class Clock>
Timestamp FromTimePoint(std::chrono::time_point<Clock> tp) {
  Timestamp ts;

  // seconds
  auto duration = tp.time_since_epoch();
  auto duration_s = std::chrono::duration_cast<std::chrono::seconds>(duration);
  ts.field_second().value() = duration_s.count();

  // fraction
  duration -= duration_s;
  auto duration_ns = \
    std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
  ts.field_fraction().value() = \
    (duration_ns.count() * std::numeric_limits<uint32_t>::max()) / 1e9;

  return ts;
}

/// Get the current timestamp for a specific clock
/// \tparam Clock the std::chrono clock to obtain the current time from
template <class ReferenceClock = oac::chrono::LocalClock>
Timestamp Now() {
  return FromTimePoint(ReferenceClock::now());
}

/// Get a time point from a timestamp
/// \param ts the dntp timestamp
/// \return the associated high resolution clock time point
TimePoint ToTimePoint(const Timestamp& ts);

/// Get the round trip delay [as defined here](https://en.wikipedia.org/wiki/Network_Time_Protocol#Clock_synchronization_algorithm)
/// \param dntp message as received after a successful communication
/// \param final_timestamp the timestamp obtained when the message got received
std::chrono::nanoseconds RoundTripDelay(const Message& msg, const Timestamp& final_timestamp);

/// Get the time offset [as defined here](https://en.wikipedia.org/wiki/Network_Time_Protocol#Clock_synchronization_algorithm)
/// \param dntp message as received after a successful communication
/// \param final_timestamp the timestamp obtained when the message got received
std::chrono::nanoseconds TimeOffset(const Message& msg,
                                    const Timestamp& final_timestamp);

}  // namespace dntp
}  // namespace oac

#endif  // OAC_DNTP_UTILS_H_
