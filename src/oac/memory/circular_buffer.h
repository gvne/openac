#ifndef OAC_MEMORY_CIRCULAR_BUFFER_H_
#define OAC_MEMORY_CIRCULAR_BUFFER_H_

#include <cstdint>
#include <vector>
#include <string>

#include <spdlog/spdlog.h>

namespace oac {
namespace mem {

template <typename T>
class CircularBuffer {
public:
  CircularBuffer(std::size_t content_size) :
    content_(content_size, 0),
    push_index_(0),
    pop_index_(0) {}
  
  std::size_t content_size() const {
    return content_.size();
  }
  
  void set_push_index(uint64_t index) {
    push_index_ = index;
    if (push_index_ < content_.size()) {
      return;
    }
    push_index_ = push_index_ % content_.size();
  }
  
  void set_pop_index(uint64_t index) {
    pop_index_ = index;
    if (pop_index_ < content_.size()) {
      return;
    }
    pop_index_ = pop_index_ % content_.size();
  }
  
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
  
  void Pop(T* data, std::size_t data_size) {
    spdlog::trace("mem - circular buffer pop at index {}", pop_index_);
    auto pop_size = std::min(data_size, content_.size() - pop_index_);
    std::copy(content_.data() + pop_index_,
              content_.data() + pop_index_ + pop_size,
              data);
    set_pop_index(pop_index_ + pop_size);
    if (pop_size == data_size) {
      return;
    }
    Pop(data + pop_size, data_size - pop_size);
  }
  
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
