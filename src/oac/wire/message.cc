#include "oac/wire/message.h"

namespace oac {
namespace wire {

Message MakeEmptyMessage(uint16_t frame_count) {
  oac::wire::Message message;
  // enable the extension
  message.field_header().field_flags().field_extension().value() = 1;
  message.field_header().field_opt_extension().setExists();
  message.field_header().field_opt_extension().value().initField_oac_extension();
  message.field_header().field_opt_extension().value().accessField_oac_extension().field_dntp_server_ipv4_address().value().resize(4);
  // allocate the content size
  message.field_content().value().resize(frame_count);
  
  return message;
}

void Parse(const uint8_t* data,
           std::size_t data_size,
           Message& message,
           std::error_code& err) {
  auto status = message.doRead(data, data_size);
  if (status == comms::ErrorStatus::Success) {
    return;
  }
  err = std::make_error_code(std::errc::protocol_error);
}

void Serialize(const Message& message,
               uint8_t* data,
               std::size_t data_size,
               std::error_code& err) {
  auto status = message.doWrite(data, data_size);
  if (status == comms::ErrorStatus::Success) {
    return;
  }
  err = std::make_error_code(std::errc::protocol_error);
}

}  // namespace wire
}  // namespace oac
