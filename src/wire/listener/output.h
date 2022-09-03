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
  virtual void Synchronize(
    std::chrono::time_point<std::chrono::system_clock> tp) = 0;
  virtual void Pop(int16_t* data, std::size_t data_size) = 0;
};

}  // namespace listener
}  // namespace wire
}  // namespace oac

#endif  // OAC_WIRE_LISTENER_OUTPUT_H_
