#include "wire/publisher.h"

#include <spdlog/spdlog.h>

#include "memory/endian.h"
#include "dntp/timestamp.h"

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

  message_.flags = Message::kDefaultFlags;
  message_.sequence_number = 0;
  message_.timestamp = 0;
  message_.ssrc = 0;

  // Extension. Specific data related to oac. Mostly used for sync
  message_.extension_id = Message::kDefaultExtensionID;
  message_.extension_length = mem::ToBigEndian(Message::kExtensionLength);
  message_.extension.version = Message::Extension::kDefaultVersion;
  message_.extension.ntp_server_port = \
    mem::ToBigEndian(dntp_server_.endpoint().port());
  auto ipv4_addr = dntp_server_.endpoint().address().to_v4().to_bytes();
  message_.extension.ntp_server_address = ipv4_addr;
  message_.extension.reference_timestamp = dntp_server_.Now().Pack();

  pending_sample_count_ = 0;
}

void Publisher::Publish(const int16_t* data, std::size_t sample_count) {
  std::size_t index = 0;

  // The first message to send should include the remaining data if any
  if (pending_sample_count_ != 0) {
    // Not enough data to send a message
    if (pending_sample_count_ + sample_count < message_.data.size()) {
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
              message_.data.data());
    // -- copy the missing ones
    auto missing_sample_count = message_.data.size() - pending_sample_count_;
    std::copy(data,
              data + missing_sample_count,
              message_.data.data() + pending_sample_count_);
    index = missing_sample_count;
    pending_sample_count_ = 0;

    Publish();
  }

  while (index + message_.data.size() < sample_count) {
    // Fill data
    std::copy(data + index,
              data + index + message_.data.size(),
              message_.data.data());
    index += message_.data.size();

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
    socket_.send_to(asio::buffer(&message_, sizeof(Message)), endpoint, 0, err);
    if (err) {
      spdlog::error("wire - Could not publish: {}", err.message());
      err.clear();
    }
  }

  // update message metadata
  message_.sequence_number += 1;
  message_.timestamp += message_.data.size();
}

}  // namespace wire
}  // namespace oac
