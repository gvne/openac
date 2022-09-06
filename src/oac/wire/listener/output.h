#ifndef OAC_WIRE_LISTENER_OUTPUT_H_
#define OAC_WIRE_LISTENER_OUTPUT_H_

#include <cstdint>
#include <string>
#include <chrono>

namespace oac {
namespace wire {
namespace listener {

class Output {
 public:

  /// Update the pop index
  /// \param tp the current time point with added latency & time offset
  virtual void Synchronize(
    std::chrono::time_point<std::chrono::system_clock> tp) = 0;

  /// Pop data from the buffer and move the pop index by data_size
  /// \param data pointer to the first value that will receive the content
  /// \param data_size the number of value to pop
  virtual void Pop(int16_t* data, std::size_t data_size) = 0;
};

}  // namespace listener
}  // namespace wire
}  // namespace oac

#endif  // OAC_WIRE_LISTENER_OUTPUT_H_
