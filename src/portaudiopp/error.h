#ifndef PORTAUDIOPP_ERROR_H_
#define PORTAUDIOPP_ERROR_H_

#include <system_error>

namespace pa {
std::error_code make_error_code(int ev);
}  // namespace std


#endif  // PORTAUDIOPP_ERROR_H_
