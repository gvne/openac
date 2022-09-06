#include "input_app_component.h"

#include <spdlog/spdlog.h>

#include "memory/endian.h"

namespace oac {

InputAppComponent::InputAppComponent(asio::io_context& context) :
  juce::AudioAppComponent(),
  dntp_server_(context),
  pub_(context, dntp_server_, 1) {
}

InputAppComponent::~InputAppComponent() { shutdownAudio(); }

void InputAppComponent::Initialize(std::error_code& err) {
  dntp_server_.Start(err);
  if (err) {
    return;
  }
  spdlog::debug("DNTP server ready at {}:{}", dntp_server_.endpoint().address().to_string(), dntp_server_.endpoint().port());
  pub_.Initialize(err);
  if (err) {
    return;
  }
  setAudioChannels(2, 0);
}

void InputAppComponent::AddSubscriber(asio::ip::udp::endpoint sub_addr) {
  pub_.channel(0).AddSubscriber(std::move(sub_addr));
}

void InputAppComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
  
  const double desired_sample_rate = 44100;
  interpolator_speed_ratio_ = sampleRate / desired_sample_rate;
  interpolator_output_.resize(samplesPerBlockExpected * interpolator_speed_ratio_);
  
  spdlog::debug("Sample rate - In: {} Out: {}", sampleRate, desired_sample_rate);
  spdlog::debug("Buffer size - In: {} Out: {}", samplesPerBlockExpected, interpolator_output_.size());
  
  interpolator_.reset();
  buffer_.reserve(samplesPerBlockExpected * interpolator_speed_ratio_);
  pub_.Reset();
}

void InputAppComponent::releaseResources() {
  buffer_.clear();
}

void InputAppComponent::getNextAudioBlock(const AudioSourceChannelInfo& source) {
  auto* device = deviceManager.getCurrentAudioDevice();

  auto activeInputChannels = device->getActiveInputChannels();
  auto source_channel_idx = activeInputChannels.findNextSetBit(0);
  if (source_channel_idx < 0) {  // not enough channels
    return;
  }
  const float* source_buffer = source.buffer->getReadPointer(source_channel_idx, source.startSample);
  
  auto output_size = source.numSamples * interpolator_speed_ratio_;
  interpolator_.process(interpolator_speed_ratio_, source_buffer, interpolator_output_.data(), output_size);

  // encode
  for (auto index = 0; index < output_size; index++) {
    int16_t value = static_cast<int16_t>(std::round(static_cast<double>(interpolator_output_[index]) * std::numeric_limits<int16_t>::max()));
    buffer_[index] = oac::mem::ToBigEndian(value);
  }

  // send
  pub_.channel(0).Publish(buffer_.data(), source.numSamples);
}

}  // namespace oac
