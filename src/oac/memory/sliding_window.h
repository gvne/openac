#ifndef OAC_MEMORY_SLIDING_WINDOW_H_
#define OAC_MEMORY_SLIDING_WINDOW_H_

#include <vector>

namespace oac {
namespace mem {

// For implementation details, see
// https://nestedsoftware.com/2019/09/26/incremental-average-and-standard-deviation-with-sliding-window-470k.176143.html
class SlidingWindow {
 public:
  SlidingWindow(std::size_t size);
  void Push(double value);
  void Reset();

  double variance() const;
  double standard_deviation() const;
  double mean() const;
  double mean_standard_error() const;
  
  std::size_t content_size() const;
  std::size_t size() const;

 private:
  std::vector<double> values_;
  std::size_t index_;
  std::size_t size_;
  
  double mean_;
  double derived_squared_;
};

}  // namespace mem
}  // namespace oac


#endif  // OAC_MEMORY_SLIDING_WINDOW_H_
