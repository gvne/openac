#ifndef CABLE_OUTPUT_APP_COMPONENT_H_
#define CABLE_OUTPUT_APP_COMPONENT_H_

#include <JuceHeader.h>

#include "oac/dntp/server.h"
#include "oac/cable/publisher.h"

namespace oac {

class InputAppComponent : public juce::AudioAppComponent {
 public:
  InputAppComponent(asio::io_context& context);
  ~InputAppComponent();

  void Initialize(std::error_code& err);
  void AddSubscriber(asio::ip::udp::endpoint sub_addr);
  void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
  void releaseResources() override;
  void getNextAudioBlock(const AudioSourceChannelInfo& source) override;

 private:
  oac::dntp::Server dntp_server_;
  oac::cable::Publisher pub_;
  std::vector<int16_t> buffer_;
  
  Interpolators::Lagrange interpolator_;
  double interpolator_speed_ratio_;
  std::vector<float> interpolator_output_;
};

}  // namespace oac

#endif  // CABLE_OUTPUT_APP_COMPONENT_H_
