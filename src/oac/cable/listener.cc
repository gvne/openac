#include "oac/cable/listener.h"
#include <spdlog/spdlog.h>

namespace oac {
namespace cable {

Listener::Listener(asio::io_context& context, uint8_t channel_count):
    context_(context),
    latency_(std::chrono::milliseconds(250)) {
  for (auto index = 0; index < channel_count; index++) {
    listeners_.emplace_back(wire::Listener(context));
  }
}

void Listener::Initialize(const std::vector<uint16_t>& channel_ports,
                          std::error_code& err) {
  origin_ = std::chrono::system_clock::now();
  hr_origin_ = std::chrono::high_resolution_clock::now();
  
  assert(channel_ports.size() == listeners_.size());
  for (auto index = 0; index < channel_ports.size(); index++) {
    listeners_[index].Initialize(channel_ports[index], [this, index](){
      // start the communication with the dntp server
      DntpConnect(listeners_[index].dntp_server_endpoint());
    }, err);
    if (err) {
      return;
    }
  }
}

void Listener::Initialize(std::error_code& err) {
  for (auto& listener : listeners_) {
    listener.Initialize([this, &listener](){
      // start the communication with the dntp server
      DntpConnect(listener.dntp_server_endpoint());
    }, err);
    if (err) {
      return;
    }
  }
}

std::chrono::nanoseconds Listener::latency() const {
  return latency_;
}

void Listener::set_latency(std::chrono::nanoseconds value) {
  latency_ = value;
}

void Listener::Pop(int16_t* data, std::size_t data_size, uint8_t channel_index) {
  listeners_[channel_index].output().Pop(data, data_size);
}

uint16_t Listener::channel_port(uint8_t index) const {
  return listeners_.at(index).port();
}

void Listener::DntpConnect(const asio::ip::udp::endpoint &dntp_server_address) {
  if (dntp_clients_.find(dntp_server_address) != dntp_clients_.end()) {
    return;  // client is already running
  }
  // initialize a client
  spdlog::debug("cable - Connecting to new dntp server at {}:{}",
    dntp_server_address.address().to_string(), dntp_server_address.port());

  auto client = std::make_unique<dntp::Client>(context_);
  std::error_code err;
  client->Start(dntp_server_address, [this, dntp_server_address](
      dntp::Client::Nanoseconds time_offset,
      dntp::Client::Nanoseconds round_trip_delay){
    spdlog::trace("cable - dntp data received");
    Synchronize(dntp_server_address, round_trip_delay, time_offset);
  }, err);
  if (err) {
    spdlog::warn("cable - Could not connect to dntp server: {}", err.message());
    return;
  }
  dntp_clients_.emplace(dntp_server_address, std::move(client));
}

void Listener::Synchronize(
    const asio::ip::udp::endpoint &dntp_server_address,
    dntp::Client::Nanoseconds round_trip_delay,
    dntp::Client::Nanoseconds time_offset) {
  auto now_hr = std::chrono::high_resolution_clock::now();
  auto delta = now_hr - hr_origin_;
  auto now = origin_ + std::chrono::duration_cast<std::chrono::system_clock::duration>(delta);
  
  // We cast to milliseconds as the system_clock point is in ms
  auto lat_ms = std::chrono::duration_cast<std::chrono::milliseconds>(latency());
  now -= lat_ms;

  // Compensate for clock synchronization
  auto to_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_offset);
  now += to_ms;

  spdlog::debug("cable - Time offset: {}ms", to_ms.count());

  for (auto& listener : listeners_) {
    if (listener.dntp_server_endpoint() != dntp_server_address) {
      continue;  // not the right dntp server
    }
    listener.output().Synchronize(now);
  }
}

}  // namespace cable
}  // namespace oac
