#ifndef OAC_MEMORY_ENDIAN_H_
#define OAC_MEMORY_ENDIAN_H_

#include <cstdint>
#include <cstddef>

namespace oac {
namespace mem {

/// Convert a value into its BigEndian relative
/// \param source a pointer to the first byte of the value
/// \param size the size in bytes of the value
/// \param destination a pointer to the first byte of the result.
/// \note the destination should have the same size as the source
void ToBigEndian(const uint8_t* source, std::size_t size, uint8_t* destination);

/// Convert a value from its BigEndian relative
/// \param source a pointer to the first byte of the value
/// \param size the size in bytes of the value
/// \param destination a pointer to the first byte of the result.
/// \note the destination should have the same size as the source
void FromBigEndian(const uint8_t* source, std::size_t size, uint8_t* destination);

/// Convert a value into its BigEndian relative
/// \tparam T the values type (should be litteral)
/// \param value
/// \return the BigEndian value
template <typename T>
T ToBigEndian(T value) {
  T retval;
  ToBigEndian(reinterpret_cast<const uint8_t*>(&value),
              sizeof(T),
              reinterpret_cast<uint8_t*>(&retval));
  return retval;
}

/// Convert a value from its BigEndian relative
/// \tparam T the values type (should be litteral)
/// \param value the BigEndian value
/// \return the converted value
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
