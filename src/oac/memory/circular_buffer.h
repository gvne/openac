#ifndef OAC_MEMORY_CIRCULAR_BUFFER_H_
#define OAC_MEMORY_CIRCULAR_BUFFER_H_

#include <cstdint>
#include <vector>
#include <string>

#include <spdlog/spdlog.h>

namespace oac {
namespace mem {

/// A circular buffer implementation [as defined here](https://en.wikipedia.org/wiki/Circular_buffer)
/// \tparam the content type
template <typename T>
class CircularBuffer {
public:
  CircularBuffer(std::size_t content_size) :
    content_(content_size, 0),
    push_index_(0),
    pop_index_(0) {}

  /// The allocated size in number of T
  std::size_t content_size() const {
    return content_.size();
  }
  
  /// The size of available data.
  /// Note: invalid if we push content_size more values than we pushed
  std::size_t size() const {
    if (push_index_ >= pop_index_) {
      return push_index_ - pop_index_;
    }
    return (content_size() - pop_index_) + push_index_;
  }

  /// \param index The index at which data should be pushed
  /// \note if larger than content_size, the value will be the rest of the
  ///   division with the content size
  void set_push_index(uint64_t index) {
    push_index_ = index;
    if (push_index_ < content_.size()) {
      return;
    }
    push_index_ = push_index_ % content_.size();
  }


  /// \param index The index at which data should be poped
  /// \note if larger than content_size, the value will be the rest of the
  ///   division with the content size
  void set_pop_index(uint64_t index) {
    pop_index_ = index;
    if (pop_index_ < content_.size()) {
      return;
    }
    pop_index_ = pop_index_ % content_.size();
  }

  /// Push data into the buffer and move the push index by data_size
  /// \param data pointer to the first value to be pushed
  /// \param data_size the number of value to push
  void Push(const T* data, std::size_t data_size) {
    spdlog::trace("mem - circular buffer push at index {}", push_index_);
    auto push_size = std::min(data_size, content_.size() - push_index_);
    std::copy(data, data + push_size, content_.data() + push_index_);
    set_push_index(push_index_ + push_size);
    if (push_size == data_size) {
      return;
    }
    Push(data + push_size, data_size - push_size);
  }

  /// Pop data from the buffer and move the pop index by data_size
  /// \param data pointer to the first value that will receive the content
  /// \param data_size the number of value to pop
  void Pop(T* data, std::size_t data_size) {
    spdlog::trace("mem - circular buffer pop at index {}", pop_index_);
    auto pop_size = std::min(data_size, content_.size() - pop_index_);
    std::copy(content_.data() + pop_index_,
              content_.data() + pop_index_ + pop_size,
              data);
    // reset values to zero when read
    memset(content_.data() + pop_index_, 0, pop_size * sizeof(T));
    set_pop_index(pop_index_ + pop_size);
    if (pop_size == data_size) {
      return;
    }
    Pop(data + pop_size, data_size - pop_size);
  }

  /// reset the content to zeros
  void Clear() {
//    push_index_ = 0;
//    pop_index_ = 0;
    memset(content_.data(), 0, content_.size() * sizeof(T));
  }

private:
  std::vector<T> content_;
  std::size_t push_index_;
  std::size_t pop_index_;

};

}  // namespace mem
}  // namespace oac

#endif  // OAC_MEMORY_CIRCULAR_BUFFER_H_
