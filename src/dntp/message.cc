#include "dntp/message.h"
#include "dntp/timestamp.h"

namespace oac {
namespace dntp {

// See https://fr.wikipedia.org/wiki/Network_Time_Protocol

std::chrono::nanoseconds Message::RoundTripDelay(uint64_t final_timestamp) const {
  auto t0 = Timestamp::Unpack(originate_timestamp).ToTimePoint();
  auto t1 = Timestamp::Unpack(receive_timestamp).ToTimePoint();
  auto t2 = Timestamp::Unpack(transmit_timestamp).ToTimePoint();
  auto t3 = Timestamp::Unpack(final_timestamp).ToTimePoint();
  
  return (t3 - t0) - (t2 - t1);
}

std::chrono::nanoseconds Message::TimeOffset(uint64_t final_timestamp) const {
  auto t0 = Timestamp::Unpack(originate_timestamp).ToTimePoint();
  auto t1 = Timestamp::Unpack(receive_timestamp).ToTimePoint();
  auto t2 = Timestamp::Unpack(transmit_timestamp).ToTimePoint();
  auto t3 = Timestamp::Unpack(final_timestamp).ToTimePoint();
  
  return ((t1 - t0) + (t2 - t3)) / 2;
}

}  // namespace dntp
}  // namespace oac

