#include "oac/dntp/message.h"
#include "oac/dntp/timestamp.h"

namespace oac {
namespace dntp {

// See https://fr.wikipedia.org/wiki/Network_Time_Protocol
namespace message {
std::chrono::nanoseconds RoundTripDelay(const Message& msg,
                                        uint64_t final_timestamp) {
  using namespace timestamp;
  auto t0 = ToTimePoint(Unpack(msg.originate_timestamp));
  auto t1 = ToTimePoint(Unpack(msg.receive_timestamp));
  auto t2 = ToTimePoint(Unpack(msg.transmit_timestamp));
  auto t3 = ToTimePoint(Unpack(final_timestamp));
  
  return (t3 - t0) - (t2 - t1);
}

std::chrono::nanoseconds TimeOffset(const Message& msg,
                                    uint64_t final_timestamp) {
  using namespace timestamp;
  auto t0 = ToTimePoint(Unpack(msg.originate_timestamp));
  auto t1 = ToTimePoint(Unpack(msg.receive_timestamp));
  auto t2 = ToTimePoint(Unpack(msg.transmit_timestamp));
  auto t3 = ToTimePoint(Unpack(final_timestamp));
  
  return ((t1 - t0) + (t2 - t3)) / 2;
}
}  // namespace message
}  // namespace dntp
}  // namespace oac

