#include "oac/wire/publisher.h"

#include <spdlog/spdlog.h>

#include "oac/memory/endian.h"
#include "oac/dntp/timestamp.h"

namespace oac {
namespace wire {

Publisher::Publisher(asio::io_context& context,
                     const dntp::ServerInterface& dntp_server) :
  socket_(context),
  dntp_server_(dntp_server),
  pending_sample_count_(0) {}

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
  header.sequence_number = 0;
  header.timestamp = 0;
  header.ssrc = 0;

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
  pending_sample_count_ = 0;
}

void Publisher::Publish(const int16_t* data, std::size_t sample_count) {
  std::size_t index = 0;

  // The first message to send should include the remaining data if any
  if (pending_sample_count_ != 0) {
    // Not enough data to send a message
    if (pending_sample_count_ + sample_count < message_.content_size()) {
      std::copy(data,
                data + sample_count,
                pending_samples_.data() + pending_sample_count_);
      pending_sample_count_ += sample_count;
      return;
    }
    // enough samples to send a message
    // -- copy the pending samples
    std::copy(pending_samples_.data(),
              pending_samples_.data() + pending_sample_count_,
              message_.content());
    // -- copy the missing ones
    auto missing_sample_count = message_.content_size() - pending_sample_count_;
    std::copy(data,
              data + missing_sample_count,
              message_.content() + pending_sample_count_);
    index = missing_sample_count;
    pending_sample_count_ = 0;

    Publish();
  }

  while (index + message_.content_size() < sample_count) {
    // Fill data
    std::copy(data + index,
              data + index + message_.content_size(),
              message_.content());
    index += message_.content_size();

    Publish();
  }
  // keep the remaining samples
  std::copy(data + index,
            data + sample_count,
            pending_samples_.data());
  pending_sample_count_ = sample_count - index;
}

void Publisher::Publish() {
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
  auto sequence_number = mem::FromBigEndian(header.sequence_number);
  sequence_number += 1;
  header.sequence_number = mem::ToBigEndian(sequence_number);
  auto timestamp = mem::FromBigEndian(header.timestamp);
  timestamp += message_.content_size();
  header.timestamp = mem::ToBigEndian(timestamp);
  message_.set_header(header);
}

}  // namespace wire
}  // namespace oac
