#ifndef OAC_WIRE_PUBLISHER_INTERFACE_H_
#define OAC_WIRE_PUBLISHER_INTERFACE_H_

#include <asio.hpp>

namespace oac {
namespace wire {

class PublisherInterface {
 public:
  virtual void AddSubscriber(asio::ip::udp::endpoint sub_addr) = 0;
  virtual void Publish(const int16_t* data, std::size_t sample_count) = 0;
};
}  // namespace wire
}  // namespace oac

#endif  // OAC_WIRE_PUBLISHER_INTERFACE_H_
