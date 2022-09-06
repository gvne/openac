#include <spdlog/spdlog.h>

#include "init.h"
#include "output_app_component.h"

int main(int argc, char* arg[]) {
  initialiseJuce_GUI();
  oac::Initialize();
  spdlog::set_level(spdlog::level::debug);
  asio::io_context context;
  std::error_code err;

  oac::OutputAppComponent ac(context);
  ac.Initialize(err);
  if (err) {
    spdlog::error("Could not initialze app component: {}", err.message());
    return -1;
  }

  spdlog::info("Listener port: {}", ac.port());

  context.run();
  return 0;
}
