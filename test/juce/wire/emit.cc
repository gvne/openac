#include <JuceHeader.h>
#include <spdlog/spdlog.h>

#include "memory/endian.h"
#include "dntp/server.h"
#include "wire/publisher.h"

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
    pub_(context, dntp_server_) {}
  
  ~AppComponent() { shutdownAudio(); }
  
  void Initialize(std::error_code& err) {
    dntp_server_.Start(err);
    if (err) {
      return;
    }
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
    pub_.AddSubscriber(std::move(sub_addr));
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
    pub_.Publish(buffer_.data(), source.numSamples);
  }
  
 private:
  oac::dntp::Server dntp_server_;
  oac::wire::Publisher pub_;
  std::vector<int16_t> buffer_;
};

int main(int argc, char* arg[]) {
//  spdlog::set_level(spdlog::level::trace);
  asio::io_context context;
  std::error_code err;
  
  AppComponent ac(context);
  ac.Initialize(err);
  if (err) {
    spdlog::error("Could not initialze app component: {}", err.message());
    return -1;
  }
  ac.AddSubscriber(asio::ip::udp::endpoint(
    asio::ip::address::from_string("127.0.0.1"), 9783));
    
  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  return 0;
}