#include "cast/receiver.h"
#include <spdlog/spdlog.h>
#include "oac/memory/endian.h"

Receiver::Receiver(int device_index) :
  device_index_(device_index),
  channel_data_(kDefaultFrameCount) {}

void Receiver::Run(std::error_code &err) {
  auto device = GetDevice(err);
  if (err) {
    return;
  }
  spdlog::debug("Using device: {}", device.name());
  
  pa::Stream stream(device);
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
