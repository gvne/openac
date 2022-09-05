#include <gtest/gtest.h>

#include <spdlog/spdlog.h>

#include "dntp/server.h"
#include "dntp/client.h"


// Mock class to simulate a difference between client and server clock
template <int32_t DeltaMilliseconds>
class UnsynchronizedClock {
 public:
  static auto now() {
    return std::chrono::system_clock::now() + std::chrono::milliseconds(DeltaMilliseconds);
  }
};

template <int32_t DeltaMilliseconds>
class UnsynchronizedServer : public oac::dntp::Server {
 public:
  using oac::dntp::Server::Server;
 protected:
  oac::dntp::Timestamp Now() const override {
    return oac::dntp::timestamp::Now<UnsynchronizedClock<DeltaMilliseconds>>();
  }
};

TEST(DNTP, Basic) {
  const int32_t expected_sync_offset_ms = 123;
  
//  spdlog::set_level(spdlog::level::trace);
  asio::io_context context;
  std::error_code err;
  
  UnsynchronizedServer<expected_sync_offset_ms> server(context);
  server.Start(err);
  ASSERT_FALSE(err);
  
  oac::dntp::Client client(context);
  // reduce the period to have an interesting test
  client.set_period(std::chrono::milliseconds(100));
  std::chrono::nanoseconds time_offset_sum;
  uint32_t time_offset_count = 0;
  auto on_update = [&time_offset_sum, &time_offset_count](std::chrono::nanoseconds time_offset, std::chrono::nanoseconds round_trip_delay){
    time_offset_sum += time_offset;
    time_offset_count += 1;
  };
  client.Start(server.endpoint(), on_update, err);
  
  ASSERT_FALSE(err);
  
  context.run_for(std::chrono::seconds(2));
  
  time_offset_sum /= time_offset_count;
  auto time_offset_ms = time_offset_sum.count() / 1e6;
  
  spdlog::info("Found offset: {}ms", time_offset_ms);
  spdlog::info("Expected offset: {}ms", expected_sync_offset_ms);
  
  // the offset mistake should be less than 10ms
  ASSERT_LT(time_offset_ms - expected_sync_offset_ms, 10);
}


