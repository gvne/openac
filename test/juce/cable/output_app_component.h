#ifndef CABLE_OUTPUT_APP_COMPONENT_H_
#define CABLE_OUTPUT_APP_COMPONENT_H_

#include <JuceHeader.h>

#include "cable/listener.h"

namespace oac {
class OutputAppComponent : public juce::AudioAppComponent {
 public:
  OutputAppComponent(asio::io_context& context);
  ~OutputAppComponent();
  void Initialize(std::vector<uint16_t> ports, std::error_code& err);
  uint16_t port() const;

  void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
  void releaseResources() override;

  void getNextAudioBlock(const AudioSourceChannelInfo& source) override;

 private:
  oac::cable::Listener listener_;
  std::vector<int16_t> buffer_;
  
  Interpolators::Lagrange interpolator_;
  double interpolator_speed_ratio_;
  std::vector<float> interpolator_input_;
};
}  // namespace oac

#endif  // CABLE_OUTPUT_APP_COMPONENT_H_
