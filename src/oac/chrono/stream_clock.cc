#include "oac/chrono/stream_clock.h"
#include <spdlog/spdlog.h>

namespace oac {
namespace chrono {

StreamClock::StreamClock(double sample_rate) :
  origin_(std::chrono::high_resolution_clock::now()),
  sample_rate_(sample_rate),
  sample_played_(0) {}

void StreamClock::Reset() {
  origin_ = std::chrono::high_resolution_clock::now();
  sample_played_ = 0;
}

void StreamClock::AddPlayedSamples(uint64_t sample_count) {
  sample_played_ += sample_count;
}

std::chrono::nanoseconds StreamClock::delay() const {
  double stream_duration_s = static_cast<double>(sample_played_) / sample_rate_;
  auto stream_duration = std::chrono::nanoseconds(static_cast<uint64_t>(stream_duration_s * 1e9));
  auto expected_stream_origin = std::chrono::high_resolution_clock::now() - stream_duration;
  return expected_stream_origin - origin_;
}

double StreamClock::delay_seconds() const {
  return static_cast<double>(delay().count()) / 1e9;
}

}  // namespace chrono
}  // namespace oac
