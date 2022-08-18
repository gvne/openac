#ifndef OAC_DNTP_MESSAGE_H_
#define OAC_DNTP_MESSAGE_H_

#include <cstdint>
#include <chrono>

namespace oac {
namespace dntp {

struct Message {
  static const uint8_t kVersion = 1;
  
  uint8_t version;
  uint64_t originate_timestamp;
  uint64_t receive_timestamp;
  uint64_t transmit_timestamp;
  
  std::chrono::nanoseconds RoundTripDelay(uint64_t final_timestamp) const;
  std::chrono::nanoseconds TimeOffset(uint64_t final_timestamp) const;
} __attribute__((packed));

}  // namespace dntp
}  // namespace oac

#endif  // OAC_DNTP_MESSAGE_H_
