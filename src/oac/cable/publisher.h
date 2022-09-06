#ifndef OAC_CABLE_PUBLISHER_H_
#define OAC_CABLE_PUBLISHER_H_

#include "oac/dntp/server_interface.h"
#include "oac/wire/publisher.h"

namespace oac {
namespace cable {

class Publisher {
 public:
  Publisher(asio::io_context& context,
            const dntp::ServerInterface& dntp_server,
            uint8_t channel_count);
  void Initialize(std::error_code& err);

  // Clear the samples to be sent & reset the synchronization point To the now
  void Reset();

  using Channel = wire::PublisherInterface;
  Channel& channel(uint8_t channel_index);
  uint8_t channel_count() const;

 private:
  const dntp::ServerInterface& dntp_server_;
  std::vector<wire::Publisher> publishers_;
};

}  // namespace cable
}  // namespace oac

#endif  // OAC_CABLE_PUBLISHER_H_
