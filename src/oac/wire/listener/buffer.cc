#include "oac/wire/listener/buffer.h"

#include <cmath>
#include <limits>
#include <spdlog/spdlog.h>

namespace oac {
namespace wire {
namespace listener {

Buffer::Buffer(const std::chrono::seconds& duration, double sample_rate) :
  data_(duration.count() * sample_rate),
  sample_rate_(sample_rate)
  {}

void Buffer::Synchronize(
  std::chrono::time_point<std::chrono::system_clock> tp) {
  auto ts = dntp::FromTimePoint(tp);
  auto pop_index = IndexFromTimestamp(ts);
  data_.set_pop_index(pop_index);
}

void Buffer::Pop(int16_t* data, std::size_t data_size) {
  data_.Pop(data, data_size);
}

void Buffer::set_push_index(const dntp::Timestamp& reference_timestamp, uint32_t rtp_timestamp) {
  auto index = IndexFromTimestamp(reference_timestamp);
  data_.set_push_index(index + rtp_timestamp);
}

void Buffer::Push(const int16_t* data, std::size_t data_size) {
  data_.Push(data, data_size);
}

void Buffer::Clear() {
  data_.Clear();
}


uint64_t Buffer::IndexFromTimestamp(const dntp::Timestamp& ts) const {
  uint64_t second_index = (static_cast<double>(ts.field_second().value()) * sample_rate_);
  double fraction = static_cast<double>(ts.field_fraction().value()) / std::numeric_limits<uint32_t>::max();
  auto fraction_index = std::lround(fraction * sample_rate_);
  return second_index + fraction_index;
}

}  // namespace listener
}  // namespace wire
}  // namespace oac
