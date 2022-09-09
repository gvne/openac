#include "oac/wire/listener/buffer.h"

#include <cmath>
#include <limits>
#include <spdlog/spdlog.h>

#include "oac/dntp/timestamp.h"

namespace oac {
namespace wire {
namespace listener {

Buffer::Buffer(std::size_t content_size) : data_(content_size) {}

void Buffer::Synchronize(
  std::chrono::time_point<std::chrono::system_clock> tp) {
  auto ts = dntp::timestamp::Pack(dntp::timestamp::FromTimePoint(tp));
  auto pop_index = IndexFromTimestamp(ts);
  data_.set_pop_index(pop_index);
}

void Buffer::Pop(int16_t* data, std::size_t data_size) {
  data_.Pop(data, data_size);
}

void Buffer::set_push_index(uint64_t reference_timestamp, uint32_t rtp_timestamp) {
  auto index = IndexFromTimestamp(reference_timestamp);
  data_.set_push_index(index + rtp_timestamp);
}

void Buffer::Push(const int16_t* data, std::size_t data_size) {
  data_.Push(data, data_size);
}

void Buffer::Clear() {
  data_.Clear();
}


uint64_t Buffer::IndexFromTimestamp(uint64_t packed_ts) const {
  auto ts = dntp::timestamp::Unpack(packed_ts);
  uint64_t second_index = (static_cast<double>(ts.seconds) * 44100);
  double fraction = static_cast<double>(ts.fraction) / std::numeric_limits<uint32_t>::max();
  auto fraction_index = std::lround(fraction * 44100);
  return second_index + fraction_index;
}

}  // namespace listener
}  // namespace wire
}  // namespace oac
