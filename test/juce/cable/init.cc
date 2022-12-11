#include "init.h"

#ifdef WIN

#include <combaseapi.h>

namespace oac {
void Initialize() {
  CoInitializeEx(0, COINIT_MULTITHREADED);
}
}  // namespace oac

#else

namespace oac {
void Initialize() {}
}  // namespace oac

#endif  // WINDOWS
