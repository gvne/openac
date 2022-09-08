#include "portaudiopp/device.h"

#include <spdlog/spdlog.h>
#include "portaudio.h"

#include "portaudiopp/error.h"

namespace pa {

std::vector<Device> Device::List(std::error_code& err) {
  std::vector<Device> retval;

  auto count = Pa_GetDeviceCount();
  err = pa::make_error_code(count);
  if (err) {
    return retval;
  }

  for (auto index = 0; index < count; index++) {
    Device device(index);
    device.Initialize(err);
    if (err) {
      spdlog::debug("Device at index {} could not be initialized: {}", index, err.message());
      err.clear();
      continue;
    }
    retval.emplace_back(std::move(device));
  }

  return retval;
}

Device Device::DefaultInput(std::error_code &err) {
  auto device = Pa_GetDefaultInputDevice();
  if (device < 0) {
    err = pa::make_error_code(device);
    return Device(0);
  }
  Device dev(device);
  dev.Initialize(err);
  return dev;
}

Device Device::DefaultOutput(std::error_code &err) {
  auto device = Pa_GetDefaultOutputDevice();
  if (device < 0) {
    err = pa::make_error_code(device);
    return Device(0);
  }
  Device dev(device);
  dev.Initialize(err);
  return dev;
}

Device::Device(int index) : index_(index) {}

void Device::Initialize(std::error_code &err) {
  info_ = Pa_GetDeviceInfo(index_);
  if (info_ == nullptr) {
    err = pa::make_error_code(paInvalidDevice);
  }
}

const char* Device::name() const {
  return info_->name;
}

int Device::index() const {
  return index_;
}

int Device::max_input_channels() const {
  return info_->maxInputChannels;
}

int Device::max_output_channels() const {
  return info_->maxOutputChannels;
}

double Device::default_sample_rate() const {
  return info_->defaultSampleRate;
}


}  // namespace pa
