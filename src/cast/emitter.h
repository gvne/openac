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
  void Run(std::error_code& err);

 private:
  pa::Device GetDevice(std::error_code& err) const;
  pa::Stream GetStream(const pa::Device& device,
                       std::error_code& err) const;

  void Emit(const int16_t* data, std::size_t frame_count);
  double sample_ratio() const;

 private:
  const uint64_t kMaxFrameCount = 2048;
  const double kDesiredSampleRate = 44100;

  asio::io_context context_;

  int device_index_;
  double stream_sample_rate_;
  uint8_t stream_channel_count_;
  std::vector<int16_t> channel_data_;
  std::vector<int16_t> resampled_channel_data_;
  resample::Interpolator<int16_t> interpolator_;

  oac::dntp::Server dntp_server_;
  std::shared_ptr<oac::cable::Publisher> pub_;
};

#endif  // CAST_EMITTER_H_
