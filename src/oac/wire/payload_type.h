#ifndef OAC_WIRE_PAYLOAD_TYPE_H_
#define OAC_WIRE_PAYLOAD_TYPE_H_

#include <algorithm>
#include "oac_msg/field/RTPFlagsCommon.h"

namespace oac {
namespace wire {

using PayloadType = oac_msg::field::RTPFlagsMembersCommon::Payload_typeVal;

uint16_t FramePerPacket(PayloadType type);
double SamplingRate(PayloadType type);

}  // namespace wire
}  // namespace oac

#endif  // OAC_WIRE_PAYLOAD_TYPE_H_
