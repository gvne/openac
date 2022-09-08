#include "cast/receiver.h"
#include <spdlog/spdlog.h>
#include "oac/memory/endian.h"

Receiver::Receiver(int device_index) :
  device_index_(device_index), sample_(0) {}

void Receiver::Run(std::error_code &err) {
  auto device = GetDevice(err);
  if (err) {
    return;
  }
  spdlog::debug("Using device: {}", device.name());
  
  auto stream = GetStream(device, err);
  if (err) {
    return;
  }
  
  // Initialize the publisher
  sub_ = std::make_shared<oac::cable::Listener>(context_, stream.output_channel_count());
  std::vector<uint16_t> channel_ports;
  for (auto channel_index = 0; channel_index < stream.output_channel_count(); channel_index++) {
    channel_ports.push_back(50000 + channel_index);
  }
  
  sub_->Initialize(channel_ports, err);
  if (err) {
    return;
  }
  
  // -- Initialize the buffers
  stream_sample_rate_ = stream.sample_rate();
  stream_channel_count_ = stream.output_channel_count();
  channel_data_.resize(kMaxFrameCount);
  poped_channel_data_.resize(kMaxFrameCount * sample_ratio());
  // --
  
  stream.set_callback([this](const int16_t* input, int16_t* output, std::size_t frame_count){ Receive(output, frame_count); });
  
  stream.Start(err);
  if (err) {
    spdlog::error("Could not start the stream: {}", err.message());
    return;
  }
  
  context_.run(err);
}

double Receiver::sample_ratio() const {
  return stream_sample_rate_ / kDesiredSampleRate;
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

pa::Stream Receiver::GetStream(const pa::Device& device,
                              std::error_code& err) const {
  pa::Stream stream(device);
  stream.set_input_channel_count(0);  // We don't care about the inputs
  stream.Open(kDesiredSampleRate, kMaxFrameCount, err);
  if (err) {
    spdlog::error("Could not open the stream: {}", err.message());
    return stream;
  }
  return stream;
}

void Receiver::Receive(int16_t* data, std::size_t frame_count) {
  for (auto channel_index = 0; channel_index < stream_channel_count_; channel_index++) {
    auto poped_data_size = std::floor(frame_count * sample_ratio());
    sub_->Pop(poped_channel_data_.data(), poped_data_size, channel_index);

    // Convert data from big endian
    for (auto& sample : poped_channel_data_) {
      sample = oac::mem::FromBigEndian(sample);
    }

    auto& channel_data = poped_channel_data_;

    // Ineterplate 44100Hz data to desired rate
    if (stream_sample_rate_ != kDesiredSampleRate) {
      interpolator_.Run(poped_channel_data_.data(), poped_data_size, channel_data_.data(), frame_count);
      channel_data = channel_data_;
    }

    // copy data
    for (auto sample_index = 0; sample_index < frame_count; sample_index++) {
      data[channel_index + sample_index * stream_channel_count_] = channel_data[sample_index];
    }
  }
  sample_ += frame_count;
}
