#include "portaudiopp/stream.h"

#include <cassert>

#include "portaudio.h"
#include <spdlog/spdlog.h>

#include "portaudiopp/error.h"

namespace pa {

int DefaultStreamCallback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData) {
  assert(statusFlags == 0);
  auto stream = reinterpret_cast<Stream*>(userData);
  return stream->OnData(input, timeInfo->inputBufferAdcTime,
                        output, timeInfo->outputBufferDacTime,
                        frameCount);
}

Stream::Stream(const Device& device) :
  device_index_(device.index()),
  input_channel_count_(device.max_input_channels()),
  output_channel_count_(device.max_output_channels()),
  stream_(nullptr),
  data_received_(false),
  input_latency_(device.default_low_input_latency()),
  output_latency_(device.default_low_output_latency())
  {}

Stream::~Stream() {
  if (is_running()) {
    std::error_code err;
    Stop(err);
    if (err) {
      spdlog::error("Could not stop stream: {}", err.message());
    }
  }
}

int Stream::input_channel_count() const {
  return input_channel_count_;
}

int Stream::output_channel_count() const {
  return output_channel_count_;
}

void Stream::set_input_channel_count(int v) {
  assert(!is_running());
  input_channel_count_ = v;
}

void Stream::set_output_channel_count(int v) {
  assert(!is_running());
  output_channel_count_ = v;
}

void Stream::Open(double desired_sample_rate, std::error_code &err) {
  std::shared_ptr<PaStreamParameters> input_stream_parameters;
  if (input_channel_count() != 0) {
    input_stream_parameters = std::make_shared<PaStreamParameters>();
    input_stream_parameters->channelCount = input_channel_count();
    input_stream_parameters->device = device_index_;
    input_stream_parameters->sampleFormat = paInt16;
    input_stream_parameters->suggestedLatency = 0;
    input_stream_parameters->hostApiSpecificStreamInfo = nullptr;
  }
  std::shared_ptr<PaStreamParameters> output_stream_parameters;
  if (output_channel_count() != 0) {
    output_stream_parameters = std::make_shared<PaStreamParameters>();
    output_stream_parameters->channelCount = output_channel_count();
    output_stream_parameters->device = device_index_;
    output_stream_parameters->sampleFormat = paInt16;
    output_stream_parameters->suggestedLatency = 0;
    output_stream_parameters->hostApiSpecificStreamInfo = nullptr;
  }

  auto errc = Pa_OpenStream(
    &stream_,
    input_stream_parameters.get(),
    output_stream_parameters.get(),
    desired_sample_rate,
    paFramesPerBufferUnspecified,  // let the system decide of frame per buffer
    0,
    DefaultStreamCallback,
    this);

  err = pa::make_error_code(errc);
}

double Stream::sample_rate() const {
  auto info = Pa_GetStreamInfo(stream_);
  if (info == nullptr) {
    return 0;
  }
  return info->sampleRate;
}

void Stream::Start(std::error_code& err) {
  err = pa::make_error_code(Pa_StartStream(stream_));
}

void Stream::Stop(std::error_code& err) {
  err = pa::make_error_code(Pa_StopStream(stream_));
  stream_ = nullptr;
}

bool Stream::is_running() const {
  if (stream_ == nullptr) {
    return false;
  }
  return Pa_IsStreamActive(stream_) == 1;
}

void Stream::set_callback(Callback callback) {
  // TODO: should lock instead ?
  assert(!is_running());
  callback_ = callback;
}

int Stream::OnData(const void *vinput, double input_time,
                   void *voutput, double output_time,
                   unsigned long frameCount) {
  if (!data_received_) {
    first_input_time_ = input_time;
    first_output_time_ = output_time;
    data_received_ = true;
  }

  auto input = reinterpret_cast<const int16_t*>(vinput);
  auto output = reinterpret_cast<int16_t*>(voutput);

  callback_(input, input_time - first_input_time_,
            output, output_time - first_output_time_,
            frameCount);

  return paContinue;
}

}  // namespace pa
