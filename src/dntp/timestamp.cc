#include "dntp/timestamp.h"
#include "memory/endian.h"

namespace oac {
namespace dntp {
namespace timestamp {
uint64_t Pack(const Timestamp& ts) {
  uint64_t retval;
  
  mem::ToBigEndian(
    ts.data(),
    ts.size(),
    reinterpret_cast<uint8_t*>(&retval));
  
  return retval;
}

Timestamp Unpack(uint64_t value) {
  Timestamp retval;
  
  mem::FromBigEndian(
    reinterpret_cast<const uint8_t*>(&value),
    retval.size(),
    retval.data());
  
  return retval;
}

TimePoint ToTimePoint(const Timestamp& ts) {
  uint64_t duration_ns = static_cast<double>(ts.fraction * 1e9) / std::numeric_limits<uint32_t>::max();
  std::chrono::nanoseconds duration(duration_ns);
  
  return TimePoint(duration + std::chrono::seconds(ts.seconds));
}
}  // namespace timestamps
}  // namespace dntp
}  // namespace oac
