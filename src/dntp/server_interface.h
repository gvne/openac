#ifndef OAC_DNTP_SERVER_INTERFACE_H_
#define OAC_DNTP_SERVER_INTERFACE_H_

#include <asio.hpp>
#include "dntp/timestamp.h"

namespace oac {
namespace dntp {

class ServerInterface {
 public:
  virtual Timestamp Now() const = 0;
  virtual asio::ip::udp::endpoint endpoint() const = 0;
};

}  // namespace dntp
}  // namespace oac

#endif  // OAC_DNTP_SERVER_H_
