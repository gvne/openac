#ifndef OAC_WIRE_PUBLISHER_H_
#define OAC_WIRE_PUBLISHER_H_

#include <system_error>
#include <list>
#include <array>

#include "dntp/server_interface.h"
#include "wire/publisher_interface.h"
#include "wire/message.h"

namespace oac {
namespace wire {

class Publisher : public PublisherInterface {
 public:
  Publisher(asio::io_context& context,
            const dntp::ServerInterface& dntp_server);

  void Initialize(std::error_code& err);

  void AddSubscriber(asio::ip::udp::endpoint sub_addr) override;

  // Clear the samples to be sent & reset the synchronization point To the now
  void Reset();

  // Send 16bits Linear PCM mono encoded data
  void Publish(const int16_t* data, std::size_t sample_count) override;

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
