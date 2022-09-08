#ifndef PORTAUDIOPP_STREAM_H_
#define PORTAUDIOPP_STREAM_H_

#include "portaudiopp/device.h"

typedef void PaStream;
struct PaStreamParameters;

namespace pa {

// TODO: support other format than 16 bits signed integers
class Stream {
 public:
  using Callback = std::function<void(
    const int16_t* input, int16_t* output, std::size_t frame_count)>;

  Stream(const Device& device);
  ~Stream();

  int input_channel_count() const;
  int output_channel_count() const;
  void set_input_channel_count(int v);  // TODO: assert if stream is running
  void set_output_channel_count(int v);  // TODO: assert if stream is running

  void Open(double desired_sample_rate, uint64_t frames_per_buffer, std::error_code& err);

  double sample_rate() const;

  void Start(std::error_code& err);
  void Stop(std::error_code& err);
  bool is_running() const;
  
  void set_callback(Callback callback);
  int OnData(const void *input, void *output, unsigned long frameCount);
  
 private:
  int device_index_;
  uint8_t input_channel_count_;
  uint8_t output_channel_count_;
  Callback callback_;
  
  PaStream* stream_;
};

}  // namespace pa

#endif  // PORTAUDIOPP_STREAM_H_