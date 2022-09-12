#include "portaudiopp/error.h"

#include "portaudio.h"

namespace pa {

struct ErrorCategory : std::error_category {
  const char* name() const noexcept override;
  std::string message(int ev) const override;
};

const char* ErrorCategory::name() const noexcept {
  return "portaudio";
}

std::string ErrorCategory::message(int ev) const {
  return std::string(Pa_GetErrorText(ev));
}

const ErrorCategory theErrorCategory {};


std::error_code make_error_code(int ev) {
  return {std::min(ev, int(0)), theErrorCategory};
}

}  // namespace pa
