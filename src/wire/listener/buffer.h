#ifndef OAC_WIRE_LISTENER_BUFFER_H_
#define OAC_WIRE_LISTENER_BUFFER_H_

#include "wire/listener/output.h"
#include "memory/circular_buffer.h"

namespace oac {
namespace wire {
namespace listener {

class Buffer : public Output {
 public:
  Buffer(std::size_t content_size);
  void Synchronize(
    std::chrono::time_point<std::chrono::system_clock> tp) override;
  void Pop(int16_t* data, std::size_t data_size) override;

  void set_push_index(uint64_t reference_timestamp, uint32_t rtp_timestamp);
  void Push(const int16_t* data, std::size_t data_size);
  void Clear();
  
 private:
  uint64_t IndexFromTimestamp(uint64_t ts) const;
 private:
  mem::CircularBuffer<int16_t> data_;
};

}  // namespace listener
}  // namespace wire
}  // namespace oac

#endif  // OAC_WIRE_LISTENER_BUFFER_H_
