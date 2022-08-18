#ifndef OAC_DNTP_CLIENT_H_
#define OAC_DNTP_CLIENT_H_

#include <asio.hpp>
#include "dntp/message.h"
#include "dntp/timestamp.h"

namespace oac {
namespace dntp {

class Client {
 public:
  explicit Client(asio::io_context& context);
  
  using Nanoseconds = std::chrono::nanoseconds;
  using UpdateCallback = std::function<void(Nanoseconds, Nanoseconds)>;
  void Start(asio::ip::udp::endpoint server_addr,
             UpdateCallback callback, std::error_code& err);
  
  void set_period(std::chrono::milliseconds period);
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
};

}  // namespace dntp
}  // namespace oac

#endif  // OAC_DNTP_CLIENT_H_
