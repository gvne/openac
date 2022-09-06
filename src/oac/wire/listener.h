#ifndef OAC_WIRE_LISTENER_H_
#define OAC_WIRE_LISTENER_H_

#include <cstdint>
#include <system_error>

#include <asio.hpp>

#include "oac/memory/circular_buffer.h"

#include "oac/wire/message.h"
#include "oac/wire/listener/output.h"
#include "oac/wire/listener/buffer.h"

namespace oac {
namespace wire {

class Listener {
 public:
  static constexpr const uint64_t kSamplingRate = 44100;

  explicit Listener(asio::io_context& context);

  void Initialize(std::error_code& err);
  void Initialize(std::function<void()> on_stream_reset,
                  std::error_code& err);
  void Initialize(uint16_t port,
                  std::function<void()> on_stream_reset,
                  std::error_code& err);

  uint16_t port() const;

  listener::Output& output();
  const asio::ip::udp::endpoint& dntp_server_endpoint() const;

 private:
  void Run();

 private:
  asio::io_context& context_;
  asio::ip::udp::socket socket_;
  std::function<void()> on_stream_reset_;
  Message message_;
  
  asio::ip::udp::endpoint pub_endpoint_;
  
  asio::ip::udp::endpoint dntp_server_endpoint_;
  uint64_t reference_timestamp_;
  listener::Buffer data_;
};

}  // namespace wire
}  // namespace oac

#endif  // OAC_WIRE_LISTENER_H_
