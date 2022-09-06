#include "oac/memory/endian.h"

namespace oac {
namespace mem {

// TODO: speed this up !
void SwapEndian(const uint8_t* source, std::size_t size, uint8_t* destination) {
  for (auto index = 0; index < size; index++) {
    destination[size - 1 - index] = source[index];
  }
}
 
void ToBigEndian(const uint8_t* source, std::size_t size, uint8_t* destination) {
#if IS_BIG_ENDIAN
  memcpy(destination, source, size);
#elif IS_LITTLE_ENDIAN
  SwapEndian(source, size, destination);
#else
  // if you reach this part, the preprocessor IS_LITTLE_ENDIAN or
  // IS_BIG_ENDIAN isn't properly set
  assert(false);
#endif
}

void FromBigEndian(const uint8_t* source, std::size_t size, uint8_t* destination) {
#if IS_BIG_ENDIAN
  memcpy(destination, source, size);
#elif IS_LITTLE_ENDIAN
  SwapEndian(source, size, destination);
#else
  // if you reach this part, the preprocessor IS_LITTLE_ENDIAN or
  // IS_BIG_ENDIAN isn't properly set
  assert(false);
#endif
}

}  // namespace mem
}  // namespace oac
