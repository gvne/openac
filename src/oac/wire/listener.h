#ifndef OAC_WIRE_LISTENER_H_
#define OAC_WIRE_LISTENER_H_

#include <cstdint>
#include <system_error>

#include <asio.hpp>

#include "oac/memory/circular_buffer.h"
#include "oac/dntp/message.h"

#include "oac/wire/message.h"
#include "oac/wire/listener/output.h"
#include "oac/wire/listener/buffer.h"

namespace oac {
namespace wire {

class Listener {
 public:
  static const std::chrono::seconds kBufferDuration;
  
  /// Constructor
  /// \param context the asio::io_context on which the listener will run
  explicit Listener(asio::io_context& context);

  /// Setup the listener on random port
  /// \param err set if an error occurs
  void Initialize(std::error_code& err);

  /// Setup the listener on random port
  /// \param on_stream_reset function called every time the stream is reset
  ///   useful to reset the connection to the relative dntp server
  /// \param err set if an error occurs
  void Initialize(std::function<void()> on_stream_reset, std::error_code& err);

  /// Setup the listener on a specific port
  /// \param port the port to listen on
  /// \param on_stream_reset function called every time the stream is reset
  ///   useful to reset the connection to the relative dntp server
  /// \param err set if an error occurs
  void Initialize(uint16_t port,
                  std::function<void()> on_stream_reset,
                  std::error_code& err);

  /// \return the port the instance is listening on
  uint16_t port() const;

  /// \return the data received (if any)
  listener::Output& output();

  /// \return the endpoint to the dntp server used as reference on the received
  ///   messages
  const asio::ip::udp::endpoint& dntp_server_endpoint() const;

 private:
  void Run();

 private:
  asio::io_context& context_;
  asio::ip::udp::socket socket_;
  std::function<void()> on_stream_reset_;
  
  Message message_;
  std::vector<uint8_t> datagram_;
  std::vector<int16_t> message_data_;

  asio::ip::udp::endpoint pub_endpoint_;

  asio::ip::udp::endpoint dntp_server_endpoint_;
  dntp::Timestamp reference_timestamp_;
  listener::Buffer data_;
};

}  // namespace wire
}  // namespace oac

#endif  // OAC_WIRE_LISTENER_H_
