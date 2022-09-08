#ifndef PORTAUDIOPP_CONTEXT_H_
#define PORTAUDIOPP_CONTEXT_H_

#include <memory>

namespace pa {

class Context {
 public:
  static void Initialize();
  static void Terminate();
  
 private:
  Context() = default;
  ~Context() = default;
  
  class Impl;
  std::shared_ptr<Impl> impl_;
  
  static Context& instance();
};

}  // namespace pa

#endif  // PORTAUDIOPP_CONTEXT_H_
