#include <gtest/gtest.h>

#include <spdlog/spdlog.h>

#include "oac/dntp/server.h"
#include "oac/cable/publisher.h"
#include "oac/cable/listener.h"

TEST(Cable, Basic) {
  spdlog::set_level(spdlog::level::debug);
  const uint8_t channel_count = 1;
  asio::io_context context;
  std::error_code err;
  
  oac::dntp::Server dntp_server(context);
  dntp_server.Start(err);
  ASSERT_FALSE(err);
  
  oac::cable::Publisher pub(context, dntp_server, channel_count);
  pub.Initialize(err);
  ASSERT_FALSE(err);
  
  oac::cable::Listener listener(context, channel_count);
  listener.set_latency(std::chrono::milliseconds(100));
  listener.Initialize(err);
  ASSERT_FALSE(err);
  
  pub.channel(0).AddSubscriber(asio::ip::udp::endpoint(
    asio::ip::address::from_string("127.0.0.1"), listener.channel_port(0)));
  
  std::thread run_thread([&context](){
    context.run();
  });
  
  // publish dummy data
  std::vector<int16_t> test_data(4410, 0);
  for (auto index = 0; index < 10; index++) {
    pub.channel(0).Publish(test_data.data(), test_data.size());
  }
  
  context.stop();
  run_thread.join();
}
