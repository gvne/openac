#include "wire/listener/buffer.h"

#include <cmath>
#include <limits>
#include <spdlog/spdlog.h>

#include "dntp/timestamp.h"

namespace oac {
namespace wire {
namespace listener {

Buffer::Buffer(std::size_t content_size) : data_(content_size) {}

void Buffer::Synchronize(
  std::chrono::time_point<std::chrono::system_clock> tp) {
  auto ts = dntp::timestamp::Pack(dntp::timestamp::FromTimePoint(tp));
  data_.set_pop_index(IndexFromTimestamp(ts));
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
  // the circular buffer index is always a modulo content size.
  // We run the modulo here to avoid overflow if the timestamp is
  // too large
  auto second_index = (ts.seconds * 44100);// % data_.content_size();
  
  double fraction = static_cast<double>(ts.fraction) / std::numeric_limits<uint32_t>::max();
  auto fraction_index = std::lround(fraction * 44100);
  
  return second_index + fraction_index;
}

}  // namespace listener
}  // namespace wire
}  // namespace oac
