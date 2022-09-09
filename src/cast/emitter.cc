#include "cast/emitter.h"
#include <spdlog/spdlog.h>
#include "oac/memory/endian.h"

Emitter::Emitter(int device_index) :
  device_index_(device_index),
  dntp_server_(context_) {}

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

  auto stream = GetStream(device, err);
  if (err) {
    return;
  }

  // Initialize the publisher
  pub_ = std::make_shared<oac::cable::Publisher>(context_, dntp_server_, stream.input_channel_count());
  pub_->Initialize(err);
  if (err) {
    return;
  }

  // pub_->channel(0).AddSubscriber(asio::ip::udp::endpoint(asio::ip::address::from_string("192.168.1.10"), 50000));
  // pub_->channel(1).AddSubscriber(asio::ip::udp::endpoint(asio::ip::address::from_string("192.168.1.10"), 50001));
   pub_->channel(0).AddSubscriber(asio::ip::udp::endpoint(asio::ip::address::from_string("192.168.1.11"), 50000));
//   pub_->channel(1).AddSubscriber(asio::ip::udp::endpoint(asio::ip::address::from_string("192.168.1.11"), 50001));
//
    pub_->channel(0).AddSubscriber(asio::ip::udp::endpoint(asio::ip::address::from_string("127.0.0.1"), 50000));
//    pub_->channel(1).AddSubscriber(asio::ip::udp::endpoint(asio::ip::address::from_string("127.0.0.1"), 50001));

  // -- Initialize the buffers
  stream_sample_rate_ = stream.sample_rate();
  stream_channel_count_ = stream.input_channel_count();
  channel_data_.resize(kMaxFrameCount);
  resampled_channel_data_.resize(kMaxFrameCount * sample_ratio());
  // --

  stream.set_callback([this](const int16_t* input, double input_time, int16_t* output, double output_time, std::size_t frame_count){
    Emit(input, input_time, frame_count);
  });

  stream.Start(err);
  if (err) {
    spdlog::error("Could not start the stream: {}", err.message());
    return;
  }

  context_.run(err);
}

double Emitter::sample_ratio() const {
  return stream_sample_rate_ / kDesiredSampleRate;
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

pa::Stream Emitter::GetStream(const pa::Device& device,
                              std::error_code& err) const {
  pa::Stream stream(device);
  stream.set_output_channel_count(0);  // We don't care about the inputs
  stream.set_input_channel_count(1);  // TODO: enable stereo with an option
  stream.Open(kDesiredSampleRate, kMaxFrameCount, err);
  if (err) {
    spdlog::error("Could not open the stream: {}", err.message());
    return stream;
  }
  return stream;
}

void Emitter::Emit(const int16_t* data, double input_time, std::size_t frame_count) {
  auto now = std::chrono::system_clock::now();
  auto stream_origin = now - std::chrono::milliseconds(static_cast<int>(input_time * 1000));
  auto dntp_origin = oac::dntp::timestamp::FromTimePoint(stream_origin);
  auto packed_dntp_origin = oac::dntp::timestamp::Pack(dntp_origin);
  auto expected_timestamp = std::round(input_time * kDesiredSampleRate);
  
  for (auto channel_index = 0; channel_index < stream_channel_count_; channel_index++) {
    for (auto channel_sample_index = 0; channel_sample_index < frame_count; channel_sample_index++) {
      auto sample_index = channel_index + channel_sample_index * stream_channel_count_;
      channel_data_[channel_sample_index] = data[sample_index];
    }

    auto& data_to_send = channel_data_;
    auto data_size = frame_count;

    // Interpolate data to 44100 if necessary
    if (stream_sample_rate_ != kDesiredSampleRate) {
      auto resampled_data_size = std::floor(frame_count * sample_ratio());

      interpolator_.Run(channel_data_.data(), frame_count,
                        resampled_channel_data_.data(),
                        data_size);

      data_to_send = resampled_channel_data_;
      data_size = resampled_data_size;
    }

    // Convert data to big endian
    for (auto& sample : data_to_send) {
      sample = oac::mem::ToBigEndian(sample);
    }

    // Publish !
    pub_->channel(channel_index).set_timestamps(expected_timestamp, packed_dntp_origin);
    pub_->channel(channel_index).Publish(data_to_send.data(), data_size);
  }
}
