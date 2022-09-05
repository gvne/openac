#ifndef OAC_DNTP_MESSAGE_H_
#define OAC_DNTP_MESSAGE_H_

#include <cstdint>
#include <chrono>

#include "astruct/astruct.h"

namespace oac {
namespace dntp {

ASTRUCT(Message,
  (uint8_t, version),
  (uint64_t, originate_timestamp),
  (uint64_t, receive_timestamp),
  (uint64_t, transmit_timestamp)
);

namespace message {
  const uint8_t kVersion = 1;
  std::chrono::nanoseconds RoundTripDelay(const Message& msg,
                                          uint64_t final_timestamp);
  std::chrono::nanoseconds TimeOffset(const Message& msg,
                                      uint64_t final_timestamp);
}  // namespace message

}  // namespace dntp
}  // namespace oac

#endif  // OAC_DNTP_MESSAGE_H_
