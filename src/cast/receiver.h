#ifndef CAST_RECEIVER_H_
#define CAST_RECEIVER_H_

#include <system_error>

#include "portaudiopp/portaudiopp.h"
#include "resample/interpolator.h"

#include "oac/memory/sliding_window.h"
#include "oac/chrono/stream_clock.h"
#include "oac/cable/listener.h"

class Receiver {
 public:
  Receiver(int device_index);
  void Run(bool use_high_latency,
           int latency_ms,
           int extra_latency_ms,
           std::error_code& err);
  
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
  
  bool receive_called_;
  oac::chrono::StreamClock stream_clock_;
  oac::mem::SlidingWindow clock_delays_;
};

#endif  // CAST_RECEIVER_H_
