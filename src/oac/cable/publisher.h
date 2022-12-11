#ifndef OAC_CABLE_PUBLISHER_H_
#define OAC_CABLE_PUBLISHER_H_

#include "oac/dntp/server_interface.h"
#include "oac/wire/publisher.h"

namespace oac {
namespace cable {

class Publisher {
 public:
  /// Constructor
  /// \param context the asio::io_context on which the publisher will run
  /// \param dntp_server the dntp server used as time reference by the publisher
  /// \param channel_count the number of channels
  Publisher(asio::io_context& context,
            const dntp::ServerInterface& dntp_server,
            uint8_t channel_count);

  /// Setup the publisher
  /// \param err set if an error occurs
  void Initialize(std::error_code& err);

  // Clear the samples to be sent & reset the synchronization point To the now
  void Reset();

  using Channel = wire::PublisherInterface;

  /// Access one of the channel to define its subscriber and publish data
  /// \param channel_index
  /// \return the relative channel interface
  Channel& channel(uint8_t channel_index);

  /// \return the number of channel of the publisher
  uint8_t channel_count() const;

 private:
  const dntp::ServerInterface& dntp_server_;
  std::vector<wire::Publisher> publishers_;
};

}  // namespace cable
}  // namespace oac

#endif  // OAC_CABLE_PUBLISHER_H_
