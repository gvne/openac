#ifndef OAC_WIRE_MESSAGE_H_
#define OAC_WIRE_MESSAGE_H_

#include <cstdint>
#include <array>

namespace oac {
namespace wire {

/// Basic RTP message for OpenAC
// Specifications can be found here:
// https://en.wikipedia.org/wiki/Real-time_Transport_Protocol
// https://en.wikipedia.org/wiki/RTP_payload_formats
struct Message {
  // Note: at the moment, we do not support much of the RTP standard.
  //  We force the flags for ease of development
  // Version: RTP is version 2                  01
  // Padding: None                              0
  // Extension: Yes                             1
  // csrc count: None                           0000
  // marker: None                               0
  // Payload type: Mono Linear PCM 16bits = 11  0001011
  static const uint16_t kDefaultFlags = 0x0101000000001011;
  // Note: at the moment, we force the packet size to its default duration
  // As specified, default packet is 20ms. Using the L16 mono payload, we get
  //  20/1000 * 441000 = 882 samples
  static const uint16_t kSampleCount = 882;

  // Header
  uint16_t flags;
  uint16_t sequence_number;
  uint32_t timestamp;
  uint32_t ssrc;
  std::array<uint32_t, 0> csrc;

  // Extension
  struct Extension {
    static const uint8_t kDefaultVersion = 1;
    
    uint8_t version;
    uint8_t reserved;  // unused data reserved for future use
    uint16_t ntp_server_port;
    std::array<uint8_t, 4> ntp_server_address;
    // the date of the first sent sample of the stream in NTP format
    uint64_t reference_timestamp;
  } __attribute__((packed));
  // Extension ID should be determined when creating the RTP session.
  // At the moment, we always consider the session open.
  // We therefore hardcode the extension ID.
  // The ID is decided by summing the letter index of the words "oac" & "wire"
  //  i.e 19 & 55 concatenated in binary format
  //  19 = 00010011
  //  55 = 00110111
  static const uint16_t kDefaultExtensionID = 0x0001001100110111;
  // Length of data in 32 bits units
  static const uint16_t kExtensionLength = sizeof(Extension) / sizeof(uint32_t);

  uint16_t extension_id;
  uint16_t extension_length;
  Extension extension;

  // Data
  std::array<int16_t, kSampleCount> data;
} __attribute__((packed));

}  // namespace wire
}  // namespace oac

#endif  // OAC_WIRE_MESSAGE_H_
