#include "dntp/timestamp.h"
#include "memory/endian.h"

namespace oac {
namespace dntp {

uint64_t Timestamp::Pack() const {
  uint64_t retval;
  
  mem::ToBigEndian(
    reinterpret_cast<const uint8_t*>(this),
    sizeof(Timestamp),
    reinterpret_cast<uint8_t*>(&retval));
  
  return retval;
}

Timestamp Timestamp::Unpack(uint64_t value) {
  Timestamp retval;
  
  mem::FromBigEndian(
    reinterpret_cast<const uint8_t*>(&value),
    sizeof(Timestamp),
    reinterpret_cast<uint8_t*>(&retval));
  
  return retval;
}

Timestamp::TimePoint Timestamp::ToTimePoint() const {
  uint64_t duration_ns = static_cast<double>(fraction * 1e9) / std::numeric_limits<uint32_t>::max();
  std::chrono::nanoseconds duration(duration_ns);
  
  return Timestamp::TimePoint(duration + std::chrono::seconds(seconds));
}

}  // namespace dntp
}  // namespace oac
