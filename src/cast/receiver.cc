#include "cast/receiver.h"
#include <spdlog/spdlog.h>
#include "oac/memory/endian.h"

Receiver::Receiver(int device_index) :
  device_index_(device_index),
  channel_data_(kDefaultFrameCount),
  receive_called_(false),
  is_stream_delayed_(false) {}

void Receiver::Run(bool use_high_latency,
                   int extra_latency_ms,
                   std::error_code &err) {
  auto device = GetDevice(err);
  if (err) {
    return;
  }
  spdlog::debug("Using device: {}", device.name());
  
  auto device_latency = use_high_latency ? device.default_high_output_latency() : device.default_low_output_latency();
  pa::Stream stream(device, 0, device_latency);
  stream.set_input_channel_count(0);  // We don't care about the inputs
  stream.set_output_channel_count(2);
  stream.Open(kSampleRate, err);
  if (err) {
    spdlog::error("Could not open the stream: {}", err.message());
    return;
  }
  stream_channel_count_ = stream.output_channel_count();
  spdlog::debug("Stream initialized");
  
  // Initialize the publisher
  sub_ = std::make_shared<oac::cable::Listener>(context_, stream.output_channel_count());
  
  auto device_latency_ms = device_latency * 1000 + extra_latency_ms;
  sub_->set_latency(std::chrono::milliseconds(250) - std::chrono::milliseconds(static_cast<uint64_t>(device_latency_ms)));
  std::vector<uint16_t> channel_ports;
  for (auto channel_index = 0; channel_index < stream.output_channel_count(); channel_index++) {
    channel_ports.push_back(kDefaultPort + channel_index);
  }
  
  sub_->Initialize(channel_ports, err);
  if (err) {
    return;
  }
  
  stream.set_callback([this](const int16_t* input, double input_time, int16_t* output, double output_time, std::size_t frame_count){
    Receive(output, frame_count);
  });
  
  stream.Start(err);
  if (err) {
    spdlog::error("Could not start the stream: {}", err.message());
    return;
  }
  spdlog::debug("Stream running");
  
  context_.run(err);
}

pa::Device Receiver::GetDevice(std::error_code& err) const {
  auto device_index = device_index_;
  if (device_index < 0) {
    auto device = pa::Device::DefaultOutput(err);
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

void Receiver::Receive(int16_t* data, std::size_t frame_count) {
  spdlog::trace("Receive {} frames", frame_count);
  
  // Keep the stream origin in memory
  if (!receive_called_) {
    sub_->Synchronize();
    stream_origin_ = std::chrono::high_resolution_clock::now();
    samples_since_origin_ = 0;
    receive_called_ = true;
    is_stream_delayed_ = false;
  }

  // When callback is called, determine if the stream got delay for unknown reason
  auto stream_time_s = static_cast<double>(samples_since_origin_) / kSampleRate;
  auto expected_stream_origin = std::chrono::high_resolution_clock::now() - std::chrono::nanoseconds(static_cast<uint64_t>(stream_time_s * 1e9));
  auto delta = expected_stream_origin - stream_origin_;

  // If the stream has a delay over 50ms for more than 1second, reset it
  if (delta > std::chrono::milliseconds(50)) {
    if (!is_stream_delayed_) {
      is_stream_delayed_ = true;
      first_delay_tp_ = std::chrono::high_resolution_clock::now();
    } else {
      auto delay_duration = std::chrono::high_resolution_clock::now() - first_delay_tp_;
      if (delay_duration >= std::chrono::seconds(1)) {
        spdlog::warn("Receiver stream got delayed for {}ms. Resetting", std::chrono::duration_cast<std::chrono::milliseconds>(delay_duration).count());
        receive_called_ = false;
        return;
      }
    }
  } else {
    is_stream_delayed_ = false;
  }

  samples_since_origin_ += frame_count;
  
  
  if (frame_count > channel_data_.size()) {
    // TODO: this is a problem. Allocating memory at runtime may fail
    spdlog::debug("Resizing buffers");
    channel_data_.resize(frame_count);
  }
  
  for (auto channel_index = 0; channel_index < stream_channel_count_; channel_index++) {
    sub_->Pop(channel_data_.data(), frame_count, channel_index);

    // Convert data from big endian
    for (auto sample_index = 0; sample_index < frame_count; sample_index++) {
      data[channel_index + sample_index * stream_channel_count_] = oac::mem::FromBigEndian(channel_data_[sample_index]);
    }
  }
}
