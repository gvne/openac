#ifndef CAST_EMITTER_H_
#define CAST_EMITTER_H_

#include <system_error>

#include "portaudiopp/portaudiopp.h"
#include "resample/interpolator.h"

#include "oac/dntp/server.h"
#include "oac/cable/publisher.h"

class Emitter {
 public:
  Emitter(int device_index);
  void Run(const std::vector<std::string>& addrs, std::error_code& err);

 private:
  pa::Device GetDevice(std::error_code& err) const;

  void Emit(const int16_t* data, std::size_t frame_count);

 private:
  const uint64_t kMaxFrameCount = 2048;
  const double kSampleRate = 44100;

  asio::io_context context_;

  int device_index_;
  uint8_t stream_channel_count_;
  std::vector<int16_t> channel_data_;

  oac::dntp::Server dntp_server_;
  std::shared_ptr<oac::cable::Publisher> pub_;
  
  bool emit_called_;
  std::chrono::high_resolution_clock::time_point stream_origin_;
  uint32_t samples_since_origin_;
  
  bool is_stream_delayed_;
  std::chrono::high_resolution_clock::time_point first_delay_tp_;
};

#endif  // CAST_EMITTER_H_
