#include "cast/emitter.h"
#include <spdlog/spdlog.h>
#include "oac/chrono/local_clock.h"
#include "oac/memory/endian.h"
#include "cast/receiver.h"

Emitter::Emitter(int device_index) :
  device_index_(device_index),
  channel_data_(kDefaultFrameCount),
  dntp_server_(context_),
  emit_called_(false),
  stream_clock_(kSampleRate),
  clock_delays_(100)  // TODO: find a logical size
  {}

void Emitter::Run(const std::vector<std::string>& addrs,
                  bool use_high_latency,
                  std::error_code &err) {
  dntp_server_.Start(err);
  if (err) {
    return;
  }

  auto device = GetDevice(err);
  if (err) {
    return;
  }
  spdlog::debug("Using device: {}", device.name());
  
  auto device_latency = use_high_latency ? device.default_high_input_latency() : device.default_low_input_latency();

  // Initialize the stream
  pa::Stream stream(device, device_latency, 0);
  stream.set_output_channel_count(0);  // We don't care about the inputs
  stream.set_input_channel_count(2);  // TODO: enable stereo with an option
  stream.Open(kSampleRate, err);
  if (err) {
    spdlog::error("Could not open the stream: {}", err.message());
    return;
  }
  assert(stream.sample_rate() == kSampleRate);
  stream_channel_count_ = stream.input_channel_count();
  spdlog::debug("Stream initialized");


  // Initialize the publisher
  pub_ = std::make_shared<oac::cable::Publisher>(context_, dntp_server_, stream.input_channel_count());
  pub_->Initialize(err);
  if (err) {
    return;
  }

  for (auto channel_idx = 0; channel_idx < pub_->channel_count(); channel_idx++) {
    for (const auto& endpoint : addrs) {
      pub_->channel(channel_idx).AddSubscriber(asio::ip::udp::endpoint(asio::ip::address::from_string(endpoint), Receiver::kDefaultPort + channel_idx));
    }
  }

  stream.set_callback([this](const int16_t* input, double input_time, int16_t* output, double output_time, std::size_t frame_count){
    Emit(input, frame_count);
  });

  stream.Start(err);
  if (err) {
    spdlog::error("Could not start the stream: {}", err.message());
    return;
  }
  spdlog::debug("Stream running !");

  context_.run(err);
}

pa::Device Emitter::GetDevice(std::error_code& err) const {
  auto device_index = device_index_;
  if (device_index < 0) {
    auto device = pa::Device::DefaultInput(err);
    if (err) {
      spdlog::error("Could not get default input device: {}", err.message());
      return pa::Device(0);
    }
    device_index = device.index();
  }
  pa::Device device(device_index);
  device.Initialize(err);
  if (err) {
    spdlog::error("Could not get device at index {}: {}", device_index, err.message());
    return device;
  }
  return device;
}

void Emitter::Emit(const int16_t* data, std::size_t frame_count) {
  spdlog::trace("Emit {} frames", frame_count);
  
  // -- Make sure the stream doesn't get delayed
  if (!emit_called_) {
    pub_->Reset();
    stream_clock_.Reset();
    clock_delays_.Reset();
    emit_called_ = true;
  }
  
  auto delay = stream_clock_.delay_seconds();
  if (clock_delays_.content_size() == clock_delays_.size()) {
    // If the received delay is abnormally large, reset the stream
    // also update if the mean clock delay is too large !
    auto std = clock_delays_.standard_deviation();
    auto mean = clock_delays_.mean();
    if ((std::abs(mean) > 0.05) || (std::abs(delay) > std * 100)) {
      emit_called_ = false;
      spdlog::warn("Emitter stream got delayed for {}s. Resetting (std={} mean={})", delay, std, mean);
      return;
    }
  }
  clock_delays_.Push(delay);
  stream_clock_.AddPlayedSamples(frame_count);
  // -----
  
  
  if (frame_count > channel_data_.size()) {
    // TODO: this is a problem. Allocating memory at runtime may fail
    spdlog::debug("Resizing buffers");
    channel_data_.resize(frame_count);
  }

  // Send data channel per channel
  for (auto channel_index = 0; channel_index < stream_channel_count_; channel_index++) {
    for (auto channel_sample_index = 0; channel_sample_index < frame_count; channel_sample_index++) {
      auto sample_index = channel_index + channel_sample_index * stream_channel_count_;
      channel_data_[channel_sample_index] = data[sample_index];
    }

    // Convert data to big endian
    for (auto index = 0; index < frame_count; index++) {
      channel_data_[index] = oac::mem::ToBigEndian(channel_data_[index]);
    }

    // Publish !
    pub_->channel(channel_index).Publish(channel_data_.data(), frame_count);
  }
}
