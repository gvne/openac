#ifndef OAC_MEMORY_ENDIAN_H_
#define OAC_MEMORY_ENDIAN_H_

#include <cstdint>
#include <cstddef>

namespace oac {
namespace mem {

void ToBigEndian(const uint8_t* source, std::size_t size, uint8_t* destination);

void FromBigEndian(const uint8_t* source, std::size_t size, uint8_t* destination);

}  // namespace mem
}  // namespace oac

#endif  // OAC_MEMORY_ENDIAN_H_
