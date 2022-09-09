#include "cast/emitter.h"
#include <spdlog/spdlog.h>
#include "oac/memory/endian.h"

Emitter::Emitter(int device_index) :
  device_index_(device_index),
  dntp_server_(context_),
  emit_called_(false) {}

void Emitter::Run(std::error_code &err) {
  dntp_server_.Start(err);
  if (err) {
    return;
  }

  auto device = GetDevice(err);
  if (err) {
    return;
  }
  spdlog::debug("Using device: {}", device.name());

  // Initialize the stream
  pa::Stream stream(device);
  stream.set_output_channel_count(0);  // We don't care about the inputs
  stream.set_input_channel_count(2);  // TODO: enable stereo with an option
  stream.Open(kSampleRate, kMaxFrameCount, err);
  if (err) {
    spdlog::error("Could not open the stream: {}", err.message());
    return;
  }
  assert(stream.sample_rate() == kSampleRate);
  stream_channel_count_ = stream.input_channel_count();
  channel_data_.resize(kMaxFrameCount);
  spdlog::debug("Stream initialized");
  

  // Initialize the publisher
  pub_ = std::make_shared<oac::cable::Publisher>(context_, dntp_server_, stream.input_channel_count());
  pub_->Initialize(err);
  if (err) {
    return;
  }

  // pub_->channel(0).AddSubscriber(asio::ip::udp::endpoint(asio::ip::address::from_string("192.168.1.10"), 50000));
  // pub_->channel(1).AddSubscriber(asio::ip::udp::endpoint(asio::ip::address::from_string("192.168.1.10"), 50001));
//   pub_->channel(0).AddSubscriber(asio::ip::udp::endpoint(asio::ip::address::from_string("192.168.1.11"), 50000));
//   pub_->channel(1).AddSubscriber(asio::ip::udp::endpoint(asio::ip::address::from_string("192.168.1.11"), 50001));

    pub_->channel(0).AddSubscriber(asio::ip::udp::endpoint(asio::ip::address::from_string("127.0.0.1"), 50000));
    pub_->channel(1).AddSubscriber(asio::ip::udp::endpoint(asio::ip::address::from_string("127.0.0.1"), 50001));

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
  // Keep the stream origin in memory
  if (!emit_called_) {
    pub_->Reset();
    stream_origin_ = std::chrono::high_resolution_clock::now();
    samples_since_origin_ = 0;
    emit_called_ = true;
  }
  
  // When callback is called, determine if the stream got delay for unknown reason
  auto stream_time_s = static_cast<double>(samples_since_origin_) / kSampleRate;
  auto expected_stream_origin = std::chrono::high_resolution_clock::now() - std::chrono::nanoseconds(static_cast<uint64_t>(stream_time_s * 1e9));
  auto delta = expected_stream_origin - stream_origin_;
  
  // If delayed, reset the publisher and the stream itself
  if (delta > std::chrono::milliseconds(50)) {
    spdlog::warn("Emitter stream got delayed of {}ms. Resetting", std::chrono::duration_cast<std::chrono::milliseconds>(delta).count());
    emit_called_ = false;
    return;
  }
  samples_since_origin_ += frame_count;
  
  // Send data channel per channel
  for (auto channel_index = 0; channel_index < stream_channel_count_; channel_index++) {
    for (auto channel_sample_index = 0; channel_sample_index < frame_count; channel_sample_index++) {
      auto sample_index = channel_index + channel_sample_index * stream_channel_count_;
      channel_data_[channel_sample_index] = data[sample_index];
    }

    // Convert data to big endian
    for (auto& sample : channel_data_) {
      sample = oac::mem::ToBigEndian(sample);
    }

    // Publish !
    pub_->channel(channel_index).Publish(channel_data_.data(), frame_count);
  }
}
