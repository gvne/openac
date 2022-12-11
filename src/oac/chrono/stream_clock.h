#ifndef OAC_CHRONO_STREAM_CLOCK_H_
#define OAC_CHRONO_STREAM_CLOCK_H_

#include <cstdint>
#include <chrono>

namespace oac {
namespace chrono {

class StreamClock {
 public:
  using TimePoint = std::chrono::high_resolution_clock::time_point;

  StreamClock(double sample_rate);
  
  void Reset();
  void AddPlayedSamples(uint64_t sample_count);
  std::chrono::nanoseconds delay() const;
  double delay_seconds() const;

 private:
  TimePoint origin_;
  double sample_rate_;
  uint64_t sample_played_;
};

}  // namespace chrono
}  // namespace oac

#endif  // OAC_CHRONO_STREAM_CLOCK_H_
