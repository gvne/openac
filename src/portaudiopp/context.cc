#include "portaudiopp/context.h"

#include "portaudio.h"
#include <spdlog/spdlog.h>

namespace pa {

class Context::Impl {
 public:
  Impl() {
    spdlog::debug("Initialize portaudio");
    Pa_Initialize();
  }
  
  ~Impl() {
    Pa_Terminate();
    spdlog::debug("Portaudio terminated");
  }
};

Context& Context::instance() {
  static Context context;
  return context;
}

void Context::Initialize() {
  auto& context = Context::instance();
  if (context.impl_ == nullptr) {
    context.impl_ = std::make_shared<Context::Impl>();
  }
}

void Context::Terminate() {
  Context::instance().impl_.reset();
}

}  // namespace pa
