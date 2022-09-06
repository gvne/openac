#include "output_app_component.h"
#include <spdlog/spdlog.h>
#include "memory/endian.h"


namespace oac {
OutputAppComponent::OutputAppComponent(asio::io_context& context) :
    juce::AudioAppComponent(),
    listener_(context, 1) {
  setAudioChannels(0, 2);
}

OutputAppComponent::~OutputAppComponent() { shutdownAudio(); }

void OutputAppComponent::Initialize(std::error_code& err) {
  listener_.Initialize(err);
  if (err) {
    return;
  }
}

uint16_t OutputAppComponent::port() const {
  return listener_.channel_port(0);
}

void OutputAppComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
  const double desired_sample_rate = 44100;
  interpolator_speed_ratio_ = desired_sample_rate / sampleRate;
  interpolator_input_.resize(samplesPerBlockExpected * interpolator_speed_ratio_);
  
  listener_.set_latency(std::chrono::milliseconds(100));
  buffer_.resize(samplesPerBlockExpected * interpolator_speed_ratio_);
  
  spdlog::debug("Sample rate - In: {} Out: {}", desired_sample_rate, sampleRate);
  spdlog::debug("Buffer size - In: {} Out: {}", interpolator_input_.size(), samplesPerBlockExpected);
  
  interpolator_.reset();
}

void OutputAppComponent::releaseResources() {
  buffer_.clear();
}

void OutputAppComponent::getNextAudioBlock(const AudioSourceChannelInfo& source) {
  auto* device = deviceManager.getCurrentAudioDevice();

  auto activeOutputChannels = device->getActiveOutputChannels();
  auto source_channel_idx = activeOutputChannels.findNextSetBit(0);
  if (source_channel_idx < 0) {  // not enough channels
    spdlog::error("Could not get output channel");
    return;
  }

  float* source_buffer = source.buffer->getWritePointer(source_channel_idx, source.startSample);

  // get received data
  auto desired_sample_count = source.numSamples * interpolator_speed_ratio_;
  listener_.Pop(buffer_.data(), desired_sample_count, 0);

  // decode
  for (auto index = 0; index < desired_sample_count; index++) {
    int16_t value = oac::mem::FromBigEndian(buffer_[index]);
    double decoded = static_cast<double>(value) / std::numeric_limits<int16_t>::max();
    interpolator_input_[index] = decoded;
  }
  
  interpolator_.process(interpolator_speed_ratio_, interpolator_input_.data(), source_buffer, source.numSamples);
}
}  // namespace oac
