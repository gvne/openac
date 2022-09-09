#ifndef OAC_DNTP_SERVER_H_
#define OAC_DNTP_SERVER_H_

#include <system_error>
#include <chrono>

#include <asio.hpp>

#include "oac/dntp/message.h"
#include "oac/dntp/server_interface.h"

namespace oac {
namespace dntp {

/// The dntp server implementation based on the std::chrono::system_clock
class Server : public ServerInterface {
 public:
  /// Constructor
  /// \param context the asio::io_context on which the client will run
  explicit Server(asio::io_context& context);

  /// Start the server on a random port
  /// \param err set if an error occurs
  void Start(std::error_code& err);

  /// Start the server on a specific port
  /// \param port the desired port
  /// \param err set if an error occurs
  void Start(uint16_t port, std::error_code& err);

  /// The current time in NTP timestamp format
  virtual Timestamp Now() const override;

  /// the address at which the server can be reached
  asio::ip::udp::endpoint endpoint() const override;

 private:
  void Run();

 private:
  asio::io_context& context_;
  asio::ip::udp::socket socket_;

  Message message_;
  asio::ip::udp::endpoint origin_;
  
  std::chrono::system_clock::time_point origin_time_;
  std::chrono::high_resolution_clock::time_point hr_origin_time_;
};

}  // namespace dntp
}  // namespace oac

#endif  // OAC_DNTP_SERVER_H_
