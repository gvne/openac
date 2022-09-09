#include "oac/wire/publisher.h"

#include <spdlog/spdlog.h>

#include "oac/memory/endian.h"
#include "oac/dntp/timestamp.h"

namespace oac {
namespace wire {

Publisher::Publisher(asio::io_context& context,
                     const dntp::ServerInterface& dntp_server) :
  context_(context),
  socket_(context),
  dntp_server_(dntp_server),
  buffer_(Message::kSampleCount * 100)  // TODO: find a clever way to initialize that buffer
  {}

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

  Message::Header header;
  header.flags = Message::kDefaultFlags;
  header.sequence_number = mem::ToBigEndian(uint16_t(0));
  header.timestamp = mem::ToBigEndian(uint32_t(0));
  header.ssrc = mem::ToBigEndian(uint32_t(0));

  // Extension. Specific data related to oac. Mostly used for sync
  header.extension_id = Message::kDefaultExtensionID;
  header.extension_length = mem::ToBigEndian(Message::kExtensionLength);
  header.extension_version = Message::kDefaultExtensionVersion;
  header.extension_ntp_server_port = \
    mem::ToBigEndian(dntp_server_.endpoint().port());
  auto ipv4_addr = dntp_server_.endpoint().address().to_v4().to_bytes();
  header.extension_ntp_server_address_ipv4_part0 = ipv4_addr[0];
  header.extension_ntp_server_address_ipv4_part1 = ipv4_addr[1];
  header.extension_ntp_server_address_ipv4_part2 = ipv4_addr[2];
  header.extension_ntp_server_address_ipv4_part3 = ipv4_addr[3];
  header.extension_reference_timestamp = dntp::timestamp::Pack(dntp_server_.Now());

  message_.set_header(header);
}

void Publisher::Publish(const int16_t* data, std::size_t sample_count) {
  buffer_.Push(data, sample_count);
  context_.post([this](){
    Publish();
  });
}

void Publisher::Publish() {
  while (buffer_.size() > Message::kSampleCount) {
    spdlog::trace("Publishing {}", buffer_.size());
    buffer_.Pop(message_.content(), message_.content_size());
    PublishMessage();
  }
}

void Publisher::PublishMessage() {
  spdlog::trace("wire - Publishing a new message");
  std::error_code err;

  for (const auto& endpoint : receivers_addr_) {
    spdlog::trace("wire - Sending to {}:{}", endpoint.address().to_string(), endpoint.port());
    socket_.send_to(asio::buffer(message_.data(), message_.size()), endpoint, 0, err);
    if (err) {
      spdlog::error("wire - Could not publish: {}", err.message());
      err.clear();
    }
  }

  // update message metadata
  auto header = message_.header();
  
  uint16_t sequence_number = mem::FromBigEndian(header.sequence_number);
  sequence_number += 1;
  header.sequence_number = mem::ToBigEndian(sequence_number);
  
  uint32_t timestamp = mem::FromBigEndian(header.timestamp);
  timestamp += message_.content_size();
  header.timestamp = mem::ToBigEndian(timestamp);
  
  message_.set_header(header);
}

}  // namespace wire
}  // namespace oac
