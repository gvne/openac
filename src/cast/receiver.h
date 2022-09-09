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
  
  static const uint16_t kDefaultPort = 50000;

 private:
  pa::Device GetDevice(std::error_code& err) const;
  void Receive(int16_t* data, std::size_t frame_count);

 private:
  const uint64_t kDefaultFrameCount = 4096;
  const double kSampleRate = 44100;

  asio::io_context context_;

  int device_index_;
  uint8_t stream_channel_count_;
  std::vector<int16_t> channel_data_;

  std::shared_ptr<oac::cable::Listener> sub_;
};

#endif  // CAST_RECEIVER_H_
