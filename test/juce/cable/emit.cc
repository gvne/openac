#include <spdlog/spdlog.h>

#include "init.h"
#include "input_app_component.h"


int main(int argc, char* arg[]) {
  initialiseJuce_GUI();
  oac::Initialize();
  spdlog::set_level(spdlog::level::debug);
  asio::io_context context;
  std::error_code err;

  oac::InputAppComponent ac(context);
  ac.Initialize(err);
  if (err) {
    spdlog::error("Could not initialze app component: {}", err.message());
    return -1;
  }
  ac.AddSubscriber(asio::ip::udp::endpoint(
    asio::ip::address::from_string("127.0.0.1"), 50000));

  context.run();
  return 0;
}
