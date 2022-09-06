#ifndef OAC_DNTP_MESSAGE_H_
#define OAC_DNTP_MESSAGE_H_

#include <cstdint>
#include <chrono>

#include "astruct/astruct.h"

namespace oac {
namespace dntp {

/// A basic DNTP message
/// timestamps have the same format as regular [NTP timestamps](https://en.wikipedia.org/wiki/Network_Time_Protocol#Timestamps)
ASTRUCT(Message,
  (uint8_t, version),
  (uint64_t, originate_timestamp),
  (uint64_t, receive_timestamp),
  (uint64_t, transmit_timestamp)
);

namespace message {
  const uint8_t kVersion = 1;
  /// Get the round trip delay [as defined here](https://en.wikipedia.org/wiki/Network_Time_Protocol#Clock_synchronization_algorithm)
  /// \param dntp message as received after a successful communication
  /// \param final_timestamp the timestamp obtained when the message got received
  std::chrono::nanoseconds RoundTripDelay(const Message& msg,
                                          uint64_t final_timestamp);

  /// Get the time offset [as defined here](https://en.wikipedia.org/wiki/Network_Time_Protocol#Clock_synchronization_algorithm)
  /// \param dntp message as received after a successful communication
  /// \param final_timestamp the timestamp obtained when the message got received
  std::chrono::nanoseconds TimeOffset(const Message& msg,
                                      uint64_t final_timestamp);
}  // namespace message

}  // namespace dntp
}  // namespace oac

#endif  // OAC_DNTP_MESSAGE_H_
