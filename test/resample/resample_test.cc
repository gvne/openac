#include <gtest/gtest.h>

#include <iostream>

#include "resample/interpolator.h"
#include "resample/lagrange.h"

TEST(Resample, Basic) {
  const double in_sample_rate = 48000;
  const double out_sample_rate = 44100;
  double ratio = out_sample_rate / in_sample_rate;

  // initialize the input sin wave of 2 seconds
  std::vector<double> in;
  for (auto index = 0; index < in_sample_rate * 2; index++) {
    double value = sin(2 * 3.1415 * 440 * static_cast<double>(index) / in_sample_rate);
    in.push_back(value);
  }

  std::vector<double> out(in.size() * ratio);
  std::vector<double> expected_in(in.size());

  // run the interpolation in 10 slices
  auto slice_count = 10;
  auto in_slice_size = (in.size() / slice_count);
  auto out_slice_size = (out.size() / slice_count);

  resample::Interpolator<double> forward;
  for (auto slice_index = 0; slice_index < slice_count; slice_index++) {
    auto in_index = in_slice_size * slice_index;
    auto out_index = out_slice_size * slice_index;
    forward.Run(in.data() + in_index, in_slice_size,
                out.data() + out_index, out_slice_size);
  }

  resample::Interpolator<double> backward;
  for (auto slice_index = 0; slice_index < slice_count; slice_index++) {
    auto in_index = in_slice_size * slice_index;
    auto out_index = out_slice_size * slice_index;
    backward.Run(out.data() + out_index, out_slice_size,
                 expected_in.data() + in_index, in_slice_size);
  }

  // edges may cause problems
  for (auto i = 10; i < in.size() - 10; i++) {
    ASSERT_LT(abs(in[i] - expected_in[i]), 1e-3);
  }
}
