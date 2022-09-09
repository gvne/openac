#ifndef PORTAUDIOPP_DEVICE_H_
#define PORTAUDIOPP_DEVICE_H_

#include <system_error>
#include <vector>

struct PaDeviceInfo;

namespace pa {

class Device {
 public:
  static std::vector<Device> List(std::error_code& err);
  static Device DefaultInput(std::error_code& err);
  static Device DefaultOutput(std::error_code& err);

  Device(int index);
  void Initialize(std::error_code& err);

  const char* name() const;
  int index() const;
  int max_input_channels() const;
  int max_output_channels() const;
  double default_sample_rate() const;
  
  double default_high_input_latency() const;
  double default_low_input_latency() const;
  double default_high_output_latency() const;
  double default_low_output_latency() const;

 private:
  int index_;
  const PaDeviceInfo* info_;
};

}  // namespace pa

#endif  // PORTAUDIOPP_DEVICE_H_
