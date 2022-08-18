#ifndef OAC_DNTP_SERVER_H_
#define OAC_DNTP_SERVER_H_

#include <system_error>
#include <asio.hpp>
#include "dntp/message.h"
#include "dntp/timestamp.h"

namespace oac {
namespace dntp {

class Server {
 public:
  explicit Server(asio::io_context& context);
  void Start(std::error_code& err);
  void Start(uint16_t port, std::error_code& err);

  uint16_t port() const;

 protected:
  virtual Timestamp Now() const;

 private:
  void Run();

 private:
  asio::io_context& context_;
  asio::ip::udp::socket socket_;

  Message message_;
  asio::ip::udp::endpoint origin_;
};

}  // namespace dntp
}  // namespace oac

#endif  // OAC_DNTP_SERVER_H_
