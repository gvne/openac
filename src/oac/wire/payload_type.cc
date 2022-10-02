#include "oac/wire/payload_type.h"
#include <stdexcept>

namespace oac {
namespace wire {

uint16_t FramePerPacket(PayloadType type) {
  switch (type) {
    case PayloadType::L16_mono:
      return 882;  // 20ms 44100Hz;

    default:
      throw std::invalid_argument{"Unsupported payload type"};
  }
}

double SamplingRate(PayloadType type) {
  switch (type) {
    case PayloadType::L16_mono:
      return 44100;

    default:
      throw std::invalid_argument{"Unsupported payload type"};
  }
}

}  // namespace wire
}  // namespace oac
