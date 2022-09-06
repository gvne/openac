#include "oac/wire/message.h"

namespace oac {
namespace wire {

uint8_t* Message::data() {
  return data_.data();
}

const uint8_t* Message::data() const {
  return data_.data();
}

std::size_t Message::size() const {
  return data_.size();
}

Message::Header Message::header() const {
  Header header;
  std::copy(data_.data(), data_.data() + header.size(), header.data());
  return header;
}

void Message::set_header(const Header& header) {
  std::copy(header.data(), header.data() + header.size(), data_.data());
}

int16_t* Message::content() {
  uint8_t* retval = data_.data() + Header::Size::value;
  return reinterpret_cast<int16_t*>(retval);
}

const int16_t* Message::content() const {
  return reinterpret_cast<const int16_t*>(data_.data() + Header::Size::value);
}

std::size_t Message::content_size() const {
  return kSampleCount;
}

std::array<uint8_t, 4> Message::extension_ntp_server_address_ipv4() const {
  auto h = header();
  std::array<uint8_t, 4> retval;
  retval[0] = h.extension_ntp_server_address_ipv4_part0;
  retval[1] = h.extension_ntp_server_address_ipv4_part1;
  retval[2] = h.extension_ntp_server_address_ipv4_part2;
  retval[3] = h.extension_ntp_server_address_ipv4_part3;
  return retval;
}

}  // namespace wire
}  // namespace oac
