#include "oac/chrono/local_clock.h"

namespace oac {
namespace chrono {

LocalClock::LocalClock() :
  origin_(std::chrono::system_clock::now()),
  hr_origin_(std::chrono::high_resolution_clock::now()) {}

LocalClock& LocalClock::instance() {
  static LocalClock instance;
  return instance;
}

LocalClock::TimePoint LocalClock::Now() {
  auto now_hr = std::chrono::high_resolution_clock::now();
  auto delta = now_hr - hr_origin_;
  return origin_ + std::chrono::duration_cast<TimePoint::duration>(delta);
}

LocalClock::time_point LocalClock::now() noexcept {
  return LocalClock::instance().Now();
}

const LocalClock::TimePoint& LocalClock::origin() const {
  return origin_;
}

const LocalClock::HRTimePoint& LocalClock::hr_origin() const {
  return hr_origin_;
}

}  // namespace chrono
}  // namespace oac
