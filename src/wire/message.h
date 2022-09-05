#ifndef OAC_WIRE_MESSAGE_H_
#define OAC_WIRE_MESSAGE_H_

#include <cstdint>
#include <array>

#include "astruct/astruct.h"

namespace oac {
namespace wire {

/// Basic RTP message for OpenAC
// Specifications can be found here:
// https://en.wikipedia.org/wiki/Real-time_Transport_Protocol
// https://en.wikipedia.org/wiki/RTP_payload_formats
// Note: at the moment, we do not support much of the RTP standard.
//
//  We force the flags for ease of development
// Version: RTP is version 2                  01
// Padding: None                              0
// Extension: Yes                             1
// csrc count: None                           0000
// marker: None                               0
// Payload type: Mono Linear PCM 16bits = 11  0001011
class Message {
 public:
  static const uint16_t kDefaultFlags = 0x0101000000001011;
  // Note: at the moment, we force the packet size to its default duration
  // As specified, default packet is 20ms. Using the L16 mono payload, we get
  //  20/1000 * 441000 = 882 samples
  static const uint16_t kSampleCount = 882;

  ASTRUCT(Header,
    (uint16_t, flags),
    (uint16_t, sequence_number),
    (uint32_t, timestamp),
    (uint32_t, ssrc),
    // (std::array<uint32_t, 0>; csrc),  // TODO: should be of variable sire
    (uint16_t, extension_id),
    (uint16_t, extension_length),
    (uint8_t, extension_version),
    (uint8_t, extension_reserved),  // unused data reserved for future use
    (uint16_t, extension_ntp_server_port),
    (uint8_t, extension_ntp_server_address_ipv4_part0),
    (uint8_t, extension_ntp_server_address_ipv4_part1),
    (uint8_t, extension_ntp_server_address_ipv4_part2),
    (uint8_t, extension_ntp_server_address_ipv4_part3),
    (uint64_t, extension_reference_timestamp)
  );

  // Extension ID should be determined when creating the RTP session.
  // At the moment, we always consider the session open.
  // We therefore hardcode the extension ID.
  // The ID is decided by summing the letter index of the words "oac" & "wire"
  //  i.e 19 & 55 concatenated in binary format
  //  19 = 00010011
  //  55 = 00110111
  static const uint16_t kDefaultExtensionID = 0x0001001100110111;
  // Length of data in 32 bits units
  static const uint16_t kExtensionLength = \
    sizeof(uint8_t) * 2     // version & reserved
    + sizeof(uint16_t)      // ntp_server_port
    + sizeof(uint8_t) * 4   // ntp server ipv4 addr
    + sizeof(uint64_t);     // reference timestamp
  static const uint8_t kDefaultExtensionVersion = 1;

  uint8_t* data();
  const uint8_t* data() const;
  std::size_t size() const;

  Header header() const;
  void set_header(const Header& header);

  int16_t* content();
  const int16_t* content() const;
  std::size_t content_size() const;
  
  std::array<uint8_t, 4> extension_ntp_server_address_ipv4() const;

 private:
  static const uint16_t kContentSize = kSampleCount * sizeof(int16_t);

  std::array<uint8_t, Header::Size::value + kContentSize> data_;
};

}  // namespace wire
}  // namespace oac

#endif  // OAC_WIRE_MESSAGE_H_
