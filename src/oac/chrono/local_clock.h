#ifndef OAC_CHRONO_LOCAL_CLOCK_H_
#define OAC_CHRONO_LOCAL_CLOCK_H_

#include <chrono>

namespace oac {
namespace chrono {

class LocalClock {
 public:
  using TimePoint = std::chrono::system_clock::time_point;
  using HRTimePoint = std::chrono::high_resolution_clock::time_point;
  
  static LocalClock& instance();
  TimePoint Now();
  
  const TimePoint& origin() const;
  const HRTimePoint& hr_origin() const;
  
  // ----
  // Implement std interface
  using duration = TimePoint::duration;
  using rep = TimePoint::rep;
  using period = TimePoint::period;
  using time_point = TimePoint;
  static const bool is_steady = std::chrono::high_resolution_clock::is_steady;
  static time_point now() noexcept;
  // ----
  
 private:
  LocalClock();
  ~LocalClock() = default;
  
 private:
  TimePoint origin_;
  HRTimePoint hr_origin_;
};

}  // namespace chrono
}  // namespace oac

#endif  // OAC_CHRONO_LOCAL_CLOCK_H_
