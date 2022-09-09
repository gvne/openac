#ifndef OAC_DNTP_CLIENT_H_
#define OAC_DNTP_CLIENT_H_

#include <asio.hpp>

#include "oac/dntp/message.h"
#include "oac/dntp/timestamp.h"

namespace oac {
namespace dntp {

/// Connect to a oac::dntp::Server and query it on a regular basis
class Client {
 public:
  /// Constructor
  /// \param context the asio::io_context on which the client will run
  explicit Client(asio::io_context& context);

  using Nanoseconds = std::chrono::nanoseconds;
  using UpdateCallback = std::function<void(Nanoseconds, Nanoseconds)>;
  
  /// Start the communication with the server
  /// \param server_addr the endpoint on which the server can be reached
  /// \param callback the function that shall be called after every successful
  ///   communication
  /// \param err set if an error occurs
  void Start(asio::ip::udp::endpoint server_addr,
             UpdateCallback callback, std::error_code& err);

  /// Update the frequency at which the server is queried
  /// \param period the duration between two calls
  void set_period(std::chrono::milliseconds period);

  /// The current setup period
  std::chrono::milliseconds period() const;

 protected:
  virtual Timestamp Now() const;

 private:
  void ListenResponse();
  void SendRequest();

 private:
  std::chrono::milliseconds period_;
  UpdateCallback callback_;

  asio::io_context& context_;
  asio::ip::udp::socket socket_;
  asio::steady_timer timer_;
  asio::ip::udp::endpoint server_addr_;

  Message sent_message_;
  Message received_message_;
  
  std::chrono::system_clock::time_point origin_time_;
  std::chrono::high_resolution_clock::time_point hr_origin_time_;
};

}  // namespace dntp
}  // namespace oac

#endif  // OAC_DNTP_CLIENT_H_
