#ifndef OAC_WIRE_PUBLISHER_INTERFACE_H_
#define OAC_WIRE_PUBLISHER_INTERFACE_H_

#include <asio.hpp>

namespace oac {
namespace wire {

class PublisherInterface {
 public:
  /// Add a destination to the publisher
  /// \param sub_addr the address to a oac::wire::Listener
  virtual void AddSubscriber(asio::ip::udp::endpoint sub_addr) = 0;

  /// Publish L16 mono encoded samples (should be big endian as required by rtp)
  /// \param data pointer to the first byte to publish
  /// \param sample_count the number of samples to send
  virtual void Publish(const int16_t* data,
                       std::size_t sample_count) = 0;
  
  /// Update the sent timestamp. Useful when the stream stopped for unknown reasons
  /// \param rtp the rtp timestamp (number of samples since start of stream)
  /// \param dntp the dntp timestamp (origin of the stream as obtained by oac::dntp::timestamp)
  virtual void set_timestamps(uint32_t rtp, uint64_t dntp) = 0;
};
}  // namespace wire
}  // namespace oac

#endif  // OAC_WIRE_PUBLISHER_INTERFACE_H_
