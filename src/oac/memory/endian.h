#ifndef OAC_MEMORY_ENDIAN_H_
#define OAC_MEMORY_ENDIAN_H_

#include <cstdint>
#include <cstddef>

namespace oac {
namespace mem {

void ToBigEndian(const uint8_t* source, std::size_t size, uint8_t* destination);

void FromBigEndian(const uint8_t* source, std::size_t size, uint8_t* destination);

template <typename T>
T ToBigEndian(T value) {
  T retval;
  ToBigEndian(reinterpret_cast<const uint8_t*>(&value),
              sizeof(T),
              reinterpret_cast<uint8_t*>(&retval));
  return retval;
}

template <typename T>
T FromBigEndian(T value) {
  T retval;
  FromBigEndian(reinterpret_cast<const uint8_t*>(&value),
                sizeof(T),
                reinterpret_cast<uint8_t*>(&retval));
  return retval;
}

}  // namespace mem
}  // namespace oac

#endif  // OAC_MEMORY_ENDIAN_H_
