#include "oac/wire/listener.h"
#include <spdlog/spdlog.h>

#include "oac/memory/endian.h"
#include "oac/dntp/message.h"

#include "oac/wire/message.h"
#include "oac/wire/publisher.h"

namespace oac {
namespace wire {
namespace internal {
std::array<uint8_t, 4> ToAddr(const auto& addr) {
  if (addr.size() != 4) {
    return {};
  }
  std::array<uint8_t, 4> retval;
  for (auto idx = 0; idx < retval.size(); idx++) {
    retval[idx] = addr[idx].value();
  }
  return retval;
}
}  // namespace internal

Listener::Listener(asio::io_context& context) :
  context_(context),
  socket_(context),
  data_(kBufferDuration, SamplingRate(Publisher::kPayloadType)),
  reference_timestamp_{},
  on_stream_reset_([](){})
{
  message_ = MakeEmptyMessage(FramePerPacket(Publisher::kPayloadType));
  datagram_.resize(message_.length());
  message_data_.resize(message_.field_content().value().size());
}

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
      asio::buffer(datagram_), pub_endpoint_,
      [&](const asio::error_code& cerr, std::size_t size) {
    spdlog::trace("wire - New message received");
    if (cerr) {
      spdlog::debug("wire - Reception failed {}", cerr.message());
      Run();
      return;
    }
    
    std::error_code err;
    Parse(datagram_.data(), datagram_.size(), message_, err);
    if (err) {
      spdlog::debug("wire - Invalid message received");
      Run();
      return;
    }
    
    auto& header = message_.field_header();
    auto& opt_extension = message_.field_header().field_opt_extension();
    if (!opt_extension.doesExist()) {
      spdlog::debug("wire - Missing synchronization extension");
      Run();
      return;
    }
    auto& extension = opt_extension.value().accessField_oac_extension();
    if (!extension.valid()) {
      spdlog::debug("wire - Invalid extension");
      Run();
      return;
    }

    // Update the message extension ip address if set to 0.0.0.0 to the
    //  publisher address.
    auto& dntp_address = extension.field_dntp_server_ipv4_address().value();
    if (internal::ToAddr(dntp_address) == std::array<uint8_t, 4>{0, 0, 0, 0}) {
      auto addr = pub_endpoint_.address().to_v4().to_bytes();
      for (auto idx = 0; idx < addr.size(); idx++) {
        dntp_address[idx].value() = addr[idx];
      }
    }
    
    asio::ip::udp::endpoint dntp_server_endpoint(asio::ip::address_v4(internal::ToAddr(dntp_address)), extension.field_dntp_server_port().value());

    // If the reference timestamp changed it means that the stream got reset
    // We can reset the read content
    auto has_same_endpoint = dntp_server_endpoint_ == dntp_server_endpoint;
    auto has_same_reference_timestamp = dntp::IsEqual(reference_timestamp_, extension.field_reference_timestamp());
    if (!(has_same_endpoint && has_same_reference_timestamp)) {
      spdlog::debug("wire - Reset stream");
      dntp_server_endpoint_ = dntp_server_endpoint;
      reference_timestamp_ = extension.field_reference_timestamp();
      data_.Clear();
      // execute the on_stream_reset callback asynchronously
      context_.post(on_stream_reset_);
    }

    // Push obtained data in the circular buffer
    const auto& content = message_.field_content().value();
    auto data_iterator = message_data_.begin();
    for (const auto& value : content) {
      *data_iterator = value.value();
      data_iterator++;
    }
    
    data_.set_push_index(reference_timestamp_, header.field_timestamp().value());
    data_.Push(message_data_.data(), message_data_.size());
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
