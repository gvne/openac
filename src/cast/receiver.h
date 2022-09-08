#ifndef CAST_RECEIVER_H_
#define CAST_RECEIVER_H_

#include <system_error>

#include "portaudiopp/portaudiopp.h"
#include "resample/interpolator.h"

#include "oac/cable/listener.h"

class Receiver {
 public:
  Receiver(int device_index);
  void Run(std::error_code& err);

 private:
  pa::Device GetDevice(std::error_code& err) const;
  pa::Stream GetStream(const pa::Device& device,
                       std::error_code& err) const;
  void Receive(int16_t* data, std::size_t frame_count);
  double sample_ratio() const;

 private:
  const uint64_t kMaxFrameCount = 512;
  const double kDesiredSampleRate = 44100;

  asio::io_context context_;

  int device_index_;
  double stream_sample_rate_;
  uint8_t stream_channel_count_;
  std::vector<int16_t> channel_data_;
  std::vector<int16_t> poped_channel_data_;
  resample::Interpolator<int16_t> interpolator_;

  std::shared_ptr<oac::cable::Listener> sub_;
  
  uint64_t sample_;
};

#endif  // CAST_RECEIVER_H_
