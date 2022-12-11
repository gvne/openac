#include "oac/cable/publisher.h"

namespace oac {
namespace cable {

Publisher::Publisher(asio::io_context& context,
                     const dntp::ServerInterface& dntp_server,
                     uint8_t channel_count):
    dntp_server_(dntp_server) {
  for (auto index = 0; index < channel_count; index++) {
    publishers_.emplace_back(wire::Publisher(context, dntp_server_));
  }
}

void Publisher::Initialize(std::error_code &err) {
  for (auto& pub : publishers_) {
    pub.Initialize(err);
    if (err) {
      return;
    }
  }
}

void Publisher::Reset() {
  for (auto& pub : publishers_) {
    pub.Reset();
  }
}

uint8_t Publisher::channel_count() const {
  return publishers_.size();
}

Publisher::Channel& Publisher::channel(uint8_t channel_index) {
  return publishers_.at(channel_index);
}

}  // namespace cable
}  // namespace oac
