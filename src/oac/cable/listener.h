#ifndef OAC_CABLE_LISTENER_H_
#define OAC_CABLE_LISTENER_H_

#include <map>

#include "oac/dntp/client.h"
#include "oac/wire/listener.h"

namespace oac {
namespace cable {

class Listener {
 public:
  Listener(asio::io_context& context, uint8_t channel_count);
  void Initialize(std::vector<uint16_t> channel_ports, std::error_code& err);
  void Initialize(std::error_code& err);
  std::chrono::nanoseconds latency() const;
  void set_latency(std::chrono::nanoseconds value);

  void Pop(int16_t* data, std::size_t data_size, uint8_t channel_index);
  uint16_t channel_port(uint8_t index) const;

 private:
  void DntpConnect(const asio::ip::udp::endpoint& dntp_server_address);
  void Synchronize(const asio::ip::udp::endpoint& dntp_server_address,
                   dntp::Client::Nanoseconds round_trip_delay,
                   dntp::Client::Nanoseconds time_offset);

 private:
  asio::io_context& context_;
  std::chrono::nanoseconds latency_;
  std::vector<wire::Listener> listeners_;

  std::map<asio::ip::udp::endpoint, std::unique_ptr<dntp::Client>> dntp_clients_;
};

}  // namespace cable
}  // namespace oac

#endif  // OAC_CABLE_LISTENER_H_
