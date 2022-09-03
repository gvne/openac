#include "wire/listener.h"
#include <spdlog/spdlog.h>
#include "dntp/timestamp.h"

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
      asio::buffer(&message_, sizeof(Message)), pub_endpoint_,
      [&](const asio::error_code& cerr, std::size_t size) {
    spdlog::trace("wire - New message received");
    if (cerr) {
      spdlog::debug("wire - Reception failed {}", cerr.message());
      Run();
      return;
    }

    if (message_.flags != Message::kDefaultFlags || \
        message_.extension_id != Message::kDefaultExtensionID) {
      spdlog::debug("wire - Invalid message received");
      Run();
      return;
    }

    if (message_.extension.version != Message::Extension::kDefaultVersion) {
      spdlog::debug("wire - Invalid extension version");
      Run();
      return;
    }

    // Update the message extension ip address if set to 0.0.0.0 to the
    //  publisher address.
    if (message_.extension.ntp_server_address == \
          std::array<uint8_t, 4>{0, 0, 0, 0}) {
      message_.extension.ntp_server_address = \
        pub_endpoint_.address().to_v4().to_bytes();
    }
    asio::ip::udp::endpoint dntp_server_endpoint(
      asio::ip::address_v4(message_.extension.ntp_server_address),
      message_.extension.ntp_server_port);

    // If the reference timestamp changed it means that the stream got reset
    // We can reset the read content
    if (reference_timestamp_ != message_.extension.reference_timestamp || \
        dntp_server_endpoint_ != dntp_server_endpoint) {
      spdlog::debug("wire - Reset stream");
      reference_timestamp_ = message_.extension.reference_timestamp;
      dntp_server_endpoint_ = dntp_server_endpoint;
      data_.Clear();
      // execute the on_stream_reset callback asynchronously
      context_.post(on_stream_reset_);
    }

    // Push obtained data in the circular buffer
    data_.set_push_index(reference_timestamp_, message_.timestamp);
    data_.Push(message_.data.data(), message_.data.size());

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
