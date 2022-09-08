#ifndef RESAMPLE_INTERPOLATOR_H_
#define RESAMPLE_INTERPOLATOR_H_

#include <cstdint>
#include <cmath>
#include <vector>
#include <array>
#include "resample/lagrange.h"

namespace resample {

template <typename T>
class Interpolator {
 public:
  Interpolator() :
    previous_end_{0, 0, 0},
    x_{0, 1, 2, 3, 4} {}
  
  void Run(const T* input, size_t input_size,
           T* output, size_t output_size) {
    double ratio = static_cast<double>(output_size) / input_size;
    for (auto out_index = 0; out_index < output_size; out_index++) {
      double x = static_cast<double>(out_index) / ratio;
      auto x_index = static_cast<int>(std::round(x));
      if (x_index == x) {
        output[out_index] = input[x_index];
        continue;
      }
      
      auto in_index_min = x_index - 2;
      auto in_index_max = std::min(x_index + 2, static_cast<int>(input_size - 1));
      
      x -= in_index_min;
      auto in_size = in_index_max - in_index_min;
      if (in_index_min < 0) {
        auto y_index = 0;
        for (auto in_index = in_index_min; in_index < in_index_max; in_index++) {
          if (in_index < 0) {
            y_[y_index] = previous_end_[previous_end_.size() + in_index];
          } else {
            y_[y_index] = input[in_index];
          }
          y_index++;
        }
        output[out_index] = lagrange::interp_1D(x, x_.data(), in_size, y_.data());
        continue;
      }
      
      output[out_index] = lagrange::interp_1D(x, x_.data(), in_size, input + in_index_min);
      
      std::copy(input + input_size - previous_end_.size(), input + input_size, previous_end_.data());
    }
  }
  
 private:
  std::array<T, 3> previous_end_;
  std::array<double, 5> x_;
  std::array<T, 5> y_;
};

}  // namespace resample

#endif  // RESAMPLE_INTERPOLATOR_H_
