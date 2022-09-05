#include "dntp/message.h"
#include "dntp/timestamp.h"

namespace oac {
namespace dntp {

// See https://fr.wikipedia.org/wiki/Network_Time_Protocol
namespace message {
std::chrono::nanoseconds RoundTripDelay(const Message& msg,
                                        uint64_t final_timestamp) {
  auto t0 = Timestamp::Unpack(msg.originate_timestamp).ToTimePoint();
  auto t1 = Timestamp::Unpack(msg.receive_timestamp).ToTimePoint();
  auto t2 = Timestamp::Unpack(msg.transmit_timestamp).ToTimePoint();
  auto t3 = Timestamp::Unpack(final_timestamp).ToTimePoint();
  
  return (t3 - t0) - (t2 - t1);
}

std::chrono::nanoseconds TimeOffset(const Message& msg,
                                    uint64_t final_timestamp) {
  auto t0 = Timestamp::Unpack(msg.originate_timestamp).ToTimePoint();
  auto t1 = Timestamp::Unpack(msg.receive_timestamp).ToTimePoint();
  auto t2 = Timestamp::Unpack(msg.transmit_timestamp).ToTimePoint();
  auto t3 = Timestamp::Unpack(final_timestamp).ToTimePoint();
  
  return ((t1 - t0) + (t2 - t3)) / 2;
}
}  // namespace message
}  // namespace dntp
}  // namespace oac

