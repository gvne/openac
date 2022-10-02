#ifndef OAC_WIRE_LISTENER_BUFFER_H_
#define OAC_WIRE_LISTENER_BUFFER_H_

#include "oac/wire/listener/output.h"
#include "oac/memory/circular_buffer.h"
#include "oac/dntp/message.h"

namespace oac {
namespace wire {
namespace listener {

/// A wrapper around the oac::mem::CircularBuffer
/// The push index is set using the rtp timestamp (number of samples since the
///   beginning of the stream) added to the number of sample represented by the
///   dntp timestamp obtained from the message extension
/// The pop index is set as the number of sample represented by the current time
///   plus a latency due to user configuration and the time offset between
///   client and server
class Buffer : public Output {
 public:
  Buffer(const std::chrono::seconds& duration, double sample_rate);

  /// Update the pop index
  /// \param tp the current time point with added latency & time offset
  void Synchronize(
    std::chrono::time_point<std::chrono::system_clock> tp) override;

  /// Pop data from the buffer and move the pop index by data_size
  /// \param data pointer to the first value that will receive the content
  /// \param data_size the number of value to pop
  void Pop(int16_t* data, std::size_t data_size) override;

  /// Update the push index
  /// \param reference the reference timestamp as obtained from the extension header
  /// \param rtp_timestamp the rtp_timestamp that represent the number of samples since the
  ///   stream started
  void set_push_index(const dntp::Timestamp& reference, uint32_t rtp_timestamp);

  /// Push data to the buffer and move the push index by data_size
  /// \param data pointer to the first value to be pushed
  /// \param data_size the number of value to push
  void Push(const int16_t* data, std::size_t data_size);

  /// reset the content to zeros
  void Clear();

 private:
  uint64_t IndexFromTimestamp(const dntp::Timestamp& ts) const;

 private:
  mem::CircularBuffer<int16_t> data_;
  double sample_rate_;
};

}  // namespace listener
}  // namespace wire
}  // namespace oac

#endif  // OAC_WIRE_LISTENER_BUFFER_H_
