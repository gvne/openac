#ifndef OAC_DNTP_SERVER_INTERFACE_H_
#define OAC_DNTP_SERVER_INTERFACE_H_

#include <asio.hpp>
#include "oac/dntp/utils.h"

namespace oac {
namespace dntp {

/// Interface to any DNTP server implementation
class ServerInterface {
 public:
  /// Get the current system timestamp
  virtual Timestamp Now() const = 0;

  /// Get the address at which the server can be reached
  virtual asio::ip::udp::endpoint endpoint() const = 0;
};

}  // namespace dntp
}  // namespace oac

#endif  // OAC_DNTP_SERVER_H_
