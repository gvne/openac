#include "oac/wire/publisher.h"

#include <spdlog/spdlog.h>

#include "oac/dntp/message.h"

namespace oac {
namespace wire {

const PayloadType Publisher::kPayloadType = PayloadType::L16_mono;

Publisher::Publisher(asio::io_context& context,
                     const dntp::ServerInterface& dntp_server) :
  context_(context),
  socket_(context),
  dntp_server_(dntp_server),
  message_content_(FramePerPacket(kPayloadType)),
  buffer_(message_content_.size() * 100)  // TODO: find a clever way to initialize that buffer
  {
    message_ = MakeEmptyMessage(FramePerPacket(kPayloadType));
    message_content_.resize(message_.field_content().value().size());
    datagram_.resize(message_.length());
  }

void Publisher::Initialize(std::error_code& err) {
  socket_.open(asio::ip::udp::v4(), err);
  if (err) {
    return;
  }
  Reset();
}

void Publisher::AddSubscriber(asio::ip::udp::endpoint sub_addr) {
  receivers_addr_.push_back(sub_addr);
}

void Publisher::Reset() {
  spdlog::trace("wire - Resetting the publisher");

  auto& header = message_.field_header();
  auto& extension = message_.field_header().field_opt_extension().value().accessField_oac_extension();
  
  header.field_sequence_number().value() = 0;
  header.field_timestamp().value() = 0;
  extension.field_dntp_server_port().value() = dntp_server_.endpoint().port();
  auto& addr = extension.field_dntp_server_ipv4_address().value();
  auto dntp_addr = dntp_server_.endpoint().address().to_v4().to_bytes();
  for (auto idx = 0; idx < dntp_addr.size(); idx++) {
    addr[idx].value() = dntp_addr[idx];
  }
  extension.field_reference_timestamp().value() = dntp::Now().value();
}

void Publisher::Publish(const int16_t* data, std::size_t sample_count) {
  buffer_.Push(data, sample_count);
  Publish();
}

void Publisher::Publish() {
  while (buffer_.size() >= message_content_.size()) {
    spdlog::trace("wire - Publishing {}", buffer_.size());
    buffer_.Pop(message_content_.data(), message_content_.size());
    
    auto& content = message_.field_content().value();
    auto content_iterator = message_content_.begin();
    for (auto& value : content) {
      value.value() = *content_iterator;
      content_iterator++;
    }
    PublishMessage();
  }
}

void Publisher::PublishMessage() {
  spdlog::trace("wire - Publishing a new message");
  std::error_code err;
  Serialize(message_, datagram_.data(), datagram_.size(), err);
  if (err) {
    spdlog::error("wire - Could not serialize the datagram to send");
    return;
  }

  for (const auto& endpoint : receivers_addr_) {
    spdlog::trace("wire - Sending to {}:{}", endpoint.address().to_string(), endpoint.port());
    socket_.send_to(asio::buffer(datagram_), endpoint, 0, err);
    if (err) {
      spdlog::error("wire - Could not publish: {}", err.message());
      err.clear();
    }
  }

  // update message metadata
  message_.field_header().field_sequence_number().value() += 1;
  message_.field_header().field_timestamp().value() += message_.field_content().value().size();
}

}  // namespace wire
}  // namespace oac
