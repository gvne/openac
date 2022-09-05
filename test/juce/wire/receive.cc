#include <JuceHeader.h>
#include <spdlog/spdlog.h>

#include "wire/listener.h"
#include "memory/endian.h"

#ifdef WIN
#  include <combaseapi.h>
void Initialize() {
  CoInitializeEx(0, COINIT_MULTITHREADED);
}
#else
void Initialize() {}
#endif  // WINDOWS

class AppComponent : public juce::AudioAppComponent {
 public:
  AppComponent(asio::io_context& context) :
    juce::AudioAppComponent(),
    listener_(context) {}

  ~AppComponent() { shutdownAudio(); }

  void Initialize(uint16_t port, std::error_code& err) {
    listener_.Initialize(port, [](){}, err);
    if (err) {
      return;
    }
    auto setup = deviceManager.getAudioDeviceSetup();
    setup.sampleRate = 44100;
    deviceManager.setAudioDeviceSetup(setup, false);
    setAudioChannels(0, 1);
  }

  void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override {
    assert(sampleRate == 44100);
    listener_.output().Synchronize(std::chrono::system_clock::now() - std::chrono::milliseconds(250));
    buffer_.resize(samplesPerBlockExpected);
  }

  void releaseResources() override {
    buffer_.clear();
  }

  void getNextAudioBlock(const AudioSourceChannelInfo& source) override {
    auto* device = deviceManager.getCurrentAudioDevice();

    auto activeOutputChannels = device->getActiveOutputChannels();
    auto source_channel_idx = activeOutputChannels.findNextSetBit(0);
    if (source_channel_idx < 0) {  // not enough channels
      spdlog::error("Could not get output channel");
      return;
    }

    float* outBuffer = source.buffer->getWritePointer(source_channel_idx, source.startSample);

    // get received data
    listener_.output().Pop(buffer_.data(), source.numSamples);

    // decode
    for (auto index = 0; index < source.numSamples; index++) {
      int16_t value = oac::mem::FromBigEndian(buffer_[index]);
      double decoded = static_cast<double>(value) / std::numeric_limits<int16_t>::max();
      outBuffer[index] = decoded;
    }
  }

 private:
  oac::wire::Listener listener_;
  std::vector<int16_t> buffer_;
};

int main(int argc, char* arg[]) {
  Initialize();
//  spdlog::set_level(spdlog::level::trace);
  asio::io_context context;
  std::error_code err;

  AppComponent ac(context);
  ac.Initialize(9783, err);
  if (err) {
    spdlog::error("Could not initialze app component: {}", err.message());
    return -1;
  }

  context.run();
  return 0;
}
