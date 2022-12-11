#ifndef OAC_WIRE_PUBLISHER_H_
#define OAC_WIRE_PUBLISHER_H_

#include <system_error>
#include <list>
#include <array>

#include "oac/dntp/server_interface.h"
#include "oac/memory/circular_buffer.h"

#include "oac/wire/publisher_interface.h"
#include "oac/wire/message.h"
#include "oac/wire/payload_type.h"

namespace oac {
namespace wire {

class Publisher : public PublisherInterface {
 public:
  static const PayloadType kPayloadType;;
  
  /// Constructor
  /// \param context the asio::io_context on which the publisher will run
  /// \param dntp_server the dntp server used as time reference by the publisher
  Publisher(asio::io_context& context,
            const dntp::ServerInterface& dntp_server);

  /// Setup the publisher
  /// \param err set if an error occurs
  void Initialize(std::error_code& err);

  /// Add a destination to the publisher
  /// \param sub_addr the address to a oac::wire::Listener
  void AddSubscriber(asio::ip::udp::endpoint sub_addr) override;

  // Clear the samples to be sent & reset the synchronization point To the now
  void Reset();

  /// Publish L16 mono encoded samples (should be big endian as required by rtp)
  /// \param data pointer to the first byte to publish
  /// \param sample_count the number of samples to send
  void Publish(const int16_t* data, std::size_t sample_count) override;

 private:
  void Publish();
  void PublishMessage();

 private:
  asio::io_context& context_;
  asio::ip::udp::socket socket_;
  const dntp::ServerInterface& dntp_server_;
  
  Message message_;
  std::vector<int16_t> message_content_;
  std::vector<uint8_t> datagram_;

  mem::CircularBuffer<int16_t> buffer_;
  std::list<asio::ip::udp::endpoint> receivers_addr_;

};

}  // namespace wire
}  // namespace oac

#endif  // OAC_WIRE_PUBLISHER_H_
