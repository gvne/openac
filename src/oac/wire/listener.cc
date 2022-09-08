#include "oac/wire/listener.h"
#include <spdlog/spdlog.h>

#include "oac/memory/endian.h"
#include "oac/dntp/timestamp.h"

namespace oac {
namespace wire {

Listener::Listener(asio::io_context& context) :
  context_(context),
  socket_(context),
  data_(kSamplingRate * 10),  // Keep a buffer of 10 seconds
  reference_timestamp_(0),
  on_stream_reset_([](){})
  {}

void Listener::Initialize(std::error_code &err) {
  Initialize([](){}, err);
}

void Listener::Initialize(std::function<void()> on_stream_reset,
                          std::error_code &err) {
  Initialize(0, on_stream_reset, err);
}

void Listener::Initialize(uint16_t port,
                          std::function<void()> on_stream_reset,
                          std::error_code& err) {
  on_stream_reset_ = on_stream_reset;

  socket_.open(asio::ip::udp::v4(), err);
  if (err) {
    return;
  }
  socket_.bind(
    asio::ip::udp::endpoint(asio::ip::udp::v4(), port), err);
  if (err) {
    return;
  }
  Run();
}

uint16_t Listener::port() const {
  return socket_.local_endpoint().port();
}

void Listener::Run() {
  socket_.async_receive_from(
      asio::buffer(message_.data(), message_.size()), pub_endpoint_,
      [&](const asio::error_code& cerr, std::size_t size) {
    spdlog::trace("wire - New message received");
    if (cerr) {
      spdlog::debug("wire - Reception failed {}", cerr.message());
      Run();
      return;
    }

    auto header = message_.header();
    if (header.flags != Message::kDefaultFlags || \
        header.extension_id != Message::kDefaultExtensionID) {
      spdlog::debug("wire - Invalid message received");
      Run();
      return;
    }

    if (header.extension_version != Message::kDefaultExtensionVersion) {
      spdlog::debug("wire - Invalid extension version");
      Run();
      return;
    }

    // Update the message extension ip address if set to 0.0.0.0 to the
    //  publisher address.
    auto dntp_address = message_.extension_ntp_server_address_ipv4();
    if (dntp_address == std::array<uint8_t, 4>{0, 0, 0, 0}) {
      auto addr = pub_endpoint_.address().to_v4().to_bytes();
      header.extension_ntp_server_address_ipv4_part0 = addr[0];
      header.extension_ntp_server_address_ipv4_part1 = addr[1];
      header.extension_ntp_server_address_ipv4_part2 = addr[2];
      header.extension_ntp_server_address_ipv4_part3 = addr[3];
      dntp_address = addr;
    }
    asio::ip::udp::endpoint dntp_server_endpoint(
      asio::ip::address_v4(dntp_address),
      mem::FromBigEndian(header.extension_ntp_server_port));

    // If the reference timestamp changed it means that the stream got reset
    // We can reset the read content
    auto has_same_timestamp = \
      reference_timestamp_ == header.extension_reference_timestamp;
    auto has_same_endpoint = dntp_server_endpoint_ == dntp_server_endpoint;
    if (!(has_same_timestamp && has_same_endpoint)) {
      spdlog::debug("wire - Reset stream");
      spdlog::debug("Timestamp updated {} - Endpoint updated {}",
                    !has_same_timestamp, !has_same_endpoint);
      reference_timestamp_ = header.extension_reference_timestamp;
      dntp_server_endpoint_ = dntp_server_endpoint;
      data_.Clear();
      // execute the on_stream_reset callback asynchronously
      context_.post(on_stream_reset_);
    }

    // Push obtained data in the circular buffer
    data_.set_push_index(reference_timestamp_,
                         mem::FromBigEndian(header.timestamp));
    data_.Push(message_.content(), message_.content_size());

    Run();
  });
}

listener::Output& Listener::output() {
  return data_;
}

const asio::ip::udp::endpoint& Listener::dntp_server_endpoint() const {
  return dntp_server_endpoint_;
}

}  // namespace wire
}  // namespace oac
