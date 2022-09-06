#include <gtest/gtest.h>

#include <list>
#include <numeric>

#include <spdlog/spdlog.h>

#include "oac/dntp/server.h"
#include "oac/dntp/client.h"


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
  std::list<uint64_t> time_offsets;
  std::mutex mutex;
  auto on_update = [&time_offsets, &mutex](std::chrono::nanoseconds time_offset, std::chrono::nanoseconds round_trip_delay){
    std::lock_guard<std::mutex> lg(mutex);
    auto to_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_offset);
    spdlog::info("Obtained offset: {}ms", to_ms.count());
    time_offsets.push_back(to_ms.count());
  };
  client.Start(server.endpoint(), on_update, err);
  
  ASSERT_FALSE(err);
  
  context.run_for(std::chrono::seconds(2));
  
  ASSERT_LT(0, time_offsets.size());
  auto sum = std::accumulate(time_offsets.begin(), time_offsets.end(), 0.0);
  auto avg = sum / time_offsets.size();
  
  spdlog::info("Found offset: {}ms", avg);
  spdlog::info("Expected offset: {}ms", expected_sync_offset_ms);
  
  // the offset mistake should be less than 10ms
  ASSERT_LT(avg - expected_sync_offset_ms, 10);
}

TEST(DNTP, MessageSize) {
  ASSERT_EQ(oac::dntp::Message::Size::value, 25);
}
