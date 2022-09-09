#ifndef OAC_WIRE_PUBLISHER_H_
#define OAC_WIRE_PUBLISHER_H_

#include <system_error>
#include <list>
#include <array>

#include "oac/dntp/server_interface.h"
#include "oac/wire/publisher_interface.h"
#include "oac/wire/message.h"

namespace oac {
namespace wire {

class Publisher : public PublisherInterface {
 public:
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
  
  /// Update the sent timestamp. Useful when the stream stopped for unknown reasons
  /// \param rtp the rtp timestamp (number of samples since start of stream)
  /// \param dntp the dntp timestamp (origin of the stream as obtained by oac::dntp::timestamp)
  void set_timestamps(uint32_t rtp, uint64_t dntp) override;

 private:
  void Publish();

 private:
  asio::ip::udp::socket socket_;
  const dntp::ServerInterface& dntp_server_;
  Message message_;

  std::array<int16_t, Message::kSampleCount> pending_samples_;
  std::size_t pending_sample_count_;
  std::list<asio::ip::udp::endpoint> receivers_addr_;

};

}  // namespace wire
}  // namespace oac

#endif  // OAC_WIRE_PUBLISHER_H_
