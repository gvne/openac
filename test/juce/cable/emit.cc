#include <JuceHeader.h>
#include <spdlog/spdlog.h>

#include "memory/endian.h"
#include "dntp/server.h"
#include "cable/publisher.h"

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
    dntp_server_(context),
    pub_(context, dntp_server_, 1) {}
  
  ~AppComponent() { shutdownAudio(); }
  
  void Initialize(std::error_code& err) {
    dntp_server_.Start(err);
    if (err) {
      return;
    }
    spdlog::debug("DNTP server ready at {}:{}", dntp_server_.endpoint().address().to_string(), dntp_server_.endpoint().port());
    pub_.Initialize(err);
    if (err) {
      return;
    }
    auto setup = deviceManager.getAudioDeviceSetup();
    setup.sampleRate = 44100;
    deviceManager.setAudioDeviceSetup(setup, false);
    setAudioChannels(1, 0);
  }
  
  void AddSubscriber(asio::ip::udp::endpoint sub_addr) {
    pub_.channel(0).AddSubscriber(std::move(sub_addr));
  }
    
  void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override {
    assert(sampleRate == 44100);
    buffer_.reserve(samplesPerBlockExpected);
    pub_.Reset();
  }

  void releaseResources() override {
    buffer_.clear();
  }

  void getNextAudioBlock(const AudioSourceChannelInfo& source) override {
    auto* device = deviceManager.getCurrentAudioDevice();
    
    auto activeInputChannels = device->getActiveInputChannels();
    auto source_channel_idx = activeInputChannels.findNextSetBit(0);
    if (source_channel_idx < 0) {  // not enough channels
      return;
    }
    const float* inBuffer = source.buffer->getReadPointer(source_channel_idx, source.startSample);
    
    // encode
    for (auto index = 0; index < source.numSamples; index++) {
      int16_t value = static_cast<int16_t>(std::round(static_cast<double>(inBuffer[index]) * std::numeric_limits<int16_t>::max()));
      buffer_[index] = oac::mem::ToBigEndian(value);
    }
    
    // send
    pub_.channel(0).Publish(buffer_.data(), source.numSamples);
  }
  
 private:
  oac::dntp::Server dntp_server_;
  oac::cable::Publisher pub_;
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
  ac.AddSubscriber(asio::ip::udp::endpoint(
    asio::ip::address::from_string("192.168.1.11"), 59464));
    
  context.run();
  return 0;
}
