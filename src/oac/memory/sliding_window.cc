#include "oac/memory/sliding_window.h"
#include <cmath>

namespace oac {
namespace mem {

SlidingWindow::SlidingWindow(std::size_t size) :
  values_(size, 0),
  index_(0),
  size_(0),
  mean_(0),
  derived_squared_(0) {}

void SlidingWindow::Push(double value) {
  auto poped_value = values_[index_];
  values_[index_] = value;
  
  double new_mean;
  double derived_square_inc;
  
  if (size_ < values_.size()) {
    new_mean = ((mean_ * size_) + value) / (size_ + 1);
    derived_square_inc = (value - new_mean) * (value - mean_);
    size_ += 1;
  } else {
    auto mean_increment = (value - poped_value) / size_;
    new_mean = mean_ + mean_increment;
    derived_square_inc = (value - poped_value) * (value - new_mean + poped_value - mean_);
  }
  mean_ = new_mean;
  derived_squared_ += derived_square_inc;
  index_ = (index_ + 1) % values_.size();
}

void SlidingWindow::Reset() {
  index_ = 0;
  size_ = 0;
  mean_ = 0;
  derived_squared_ = 0;
}

double SlidingWindow::variance() const {
  return derived_squared_ / size_;
}

double SlidingWindow::standard_deviation() const {
  return sqrt(variance());
}

double SlidingWindow::mean() const {
  return mean_;
}

std::size_t SlidingWindow::content_size() const {
  return size_;
}

std::size_t SlidingWindow::size() const {
  return values_.size();
}

double SlidingWindow::mean_standard_error() const {
  return standard_deviation() / sqrt(size_);
}

}  // namespace mem
}  // namespace oac
