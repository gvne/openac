#ifndef OAC_WIRE_MESSAGE_H_
#define OAC_WIRE_MESSAGE_H_

#include <system_error>

#include "oac_msg/Message.h"
#include "oac_msg/message/RTPMessage.h"

namespace oac {
namespace wire {

using Message = oac_msg::message::RTPMessage<
    oac_msg::Message<
      comms::option::app::ReadIterator<const std::uint8_t*>,
      comms::option::app::WriteIterator<std::uint8_t*>,
      comms::option::app::LengthInfoInterface
    >
  >;

Message MakeEmptyMessage(uint16_t frame_count);

void Parse(const uint8_t* data,
           std::size_t data_size,
           Message& message,
           std::error_code& err);

void Serialize(const Message& message,
               uint8_t* data,
               std::size_t data_size,
               std::error_code& err);

}  // namespace wire
}  // namespace oac

#endif  // OAC_WIRE_MESSAGE_H_
