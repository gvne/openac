#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#include "dntp/server.h"
#include "wire/publisher.h"
#include "wire/listener.h"

std::vector<int16_t> MakeTestSignal(int16_t min, int16_t max, int16_t step) {
  std::vector<int16_t> retval;
  for (auto value = min;
       value < max;
       value += step) {
    retval.push_back(value);
  }
  return retval;
}

TEST(Wire, Basic) {
  spdlog::set_level(spdlog::level::trace);
  std::error_code err;
  asio::io_context context;
  
  oac::dntp::Server server(context);
  server.Start(err);
  ASSERT_FALSE(err);
  
  oac::wire::Listener listener(context);
  listener.Initialize(err);
  ASSERT_FALSE(err);
  
  oac::wire::Publisher pub(context, server);
  pub.Initialize(err);
  ASSERT_FALSE(err);

  // Connect publisher to listener
  pub.AddSubscriber(
    asio::ip::udp::endpoint(
      asio::ip::address::from_string("127.0.0.1"), listener.port()));

  // Send !
  const int16_t min = -10000;
  const int16_t max = 10000;
  const int16_t buffer_size = 1024;
  std::vector<int16_t> sent;
  
  // Synchronize the listener at the last moment
  // leave 100ms latency (~4410 samples)
  listener.output().Synchronize(
    std::chrono::system_clock::now() - std::chrono::milliseconds(100));
  
  for (auto index = min; index < max; index += buffer_size) {
    auto sig = MakeTestSignal(index, index + buffer_size, 1);
    pub.Publish(sig.data(), sig.size());
    sent.insert(sent.end(), sig.begin(), sig.end());
  }
  
  // Run the simulation
  context.run_for(std::chrono::seconds(1));
  std::vector<int16_t> output(sent.size());
  listener.output().Pop(output.data(), output.size());
  
  // We should have around 4410 samples latency
  // Make sure we have less than 2ms error = ~100 samples
  for (auto sample_idx = 5000; sample_idx < output.size() - 5000; sample_idx++) {
    ASSERT_LT(std::abs(sent[sample_idx] - output[sample_idx + 4410]), 100);
  }
}

TEST(Wire, MessageSize) {
  oac::wire::Message message;
  ASSERT_EQ(message.size(), 1796); // 882 * sizeof(int16_t) + 32);
}
