#include <JuceHeader.h>
#include <spdlog/spdlog.h>

#include "cable/listener.h"
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
    listener_(context, 1) {}
  
  ~AppComponent() { shutdownAudio(); }
  
  void Initialize(std::error_code& err) {
    listener_.Initialize(err);
    if (err) {
      return;
    }
    auto setup = deviceManager.getAudioDeviceSetup();
    setup.sampleRate = 44100;
    deviceManager.setAudioDeviceSetup(setup, false);
    setAudioChannels(0, 1);
  }
  
  uint16_t port() const {
    return listener_.channel_port(0);
  }
    
  void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override {
    assert(sampleRate == 44100);
    listener_.set_latency(std::chrono::milliseconds(100));
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
    listener_.Pop(buffer_.data(), source.numSamples, 0);
    
    // decode
    for (auto index = 0; index < source.numSamples; index++) {
      int16_t value = oac::mem::FromBigEndian(buffer_[index]);
      double decoded = static_cast<double>(value) / std::numeric_limits<int16_t>::max();
      outBuffer[index] = decoded;
    }
  }
  
 private:
  oac::cable::Listener listener_;
  std::vector<int16_t> buffer_;
};

int main(int argc, char* arg[]) {
  spdlog::set_level(spdlog::level::debug);
  asio::io_context context;
  std::error_code err;
  
  AppComponent ac(context);
  ac.Initialize(err);
  if (err) {
    spdlog::error("Could not initialze app component: {}", err.message());
    return -1;
  }
  
  spdlog::info("Listener port: {}", ac.port());
  
  context.run();
  return 0;
}
