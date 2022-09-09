#ifndef OAC_CABLE_LISTENER_H_
#define OAC_CABLE_LISTENER_H_

#include <map>

#include "oac/dntp/client.h"
#include "oac/wire/listener.h"

namespace oac {
namespace cable {

class Listener {
 public:
  /// Constructor
  /// \param context the asio::io_context on which the listener will run
  /// \param channel_count the number of channels
  Listener(asio::io_context& context, uint8_t channel_count);

  /// Setup the listener on random ports
  /// \param err set if an error occurs
  void Initialize(std::error_code& err);

  /// Setup the listener on specific ports
  /// \param channel_ports the desired port to listen on. channel_ports.size
  ///   should be equal to channel_count
  /// \param err set if an error occurs
  void Initialize(const std::vector<uint16_t>& channel_ports,
                  std::error_code& err);

  /// Running over the network, data from the publisher will be received with a
  ///   latency. To avoid playing data that hasn't been received yet, we setup
  ///   a fixed latency to the listener
  /// \return The added latency
  std::chrono::nanoseconds latency() const;

  /// Running over the network, data from the publisher will be received with a
  ///   latency. To avoid playing data that hasn't been received yet, we setup
  ///   a fixed latency to the listener
  /// \param The added latency
  void set_latency(std::chrono::nanoseconds value);

  /// Read received data as L16 mono encoded (big endian)
  /// \param data pointer to the first value that will receive the content
  /// \param data_size the number of value to pop
  /// \param channel_index the channel to read from
  void Pop(int16_t* data, std::size_t data_size, uint8_t channel_index);

  /// Listener port
  /// \param index the channel index
  /// \return the listener port for the specified channel index
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
  std::map<asio::ip::udp::endpoint, std::chrono::nanoseconds> latencies_;
};

}  // namespace cable
}  // namespace oac

#endif  // OAC_CABLE_LISTENER_H_
