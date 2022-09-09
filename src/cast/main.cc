#include <iostream>
#include <thread>

#include <spdlog/spdlog.h>
#include <cxxopts.hpp>

#include "portaudiopp/portaudiopp.h"
#include "cast/emitter.h"
#include "cast/receiver.h"

void ListDevices() {
  std::error_code err;
  auto devices = pa::Device::List(err);
  if (err) {
    spdlog::error("Could not list devices: {}", err.message());
    return;
  }
  std::cout << "Devices (in/out channels)" << std::endl;
  for (const auto& device : devices) {
    std::cout << device.index() << " - " << device.name() << " (" << device.max_input_channels() << "/" << device.max_output_channels() << " - latency " << device.default_low_input_latency() << "/" << device.default_high_input_latency() << ")"<< std::endl;
  }
}

void Emit(int device_index, const std::vector<std::string>& addrs) {
  std::error_code err;
  Emitter emitter(device_index);
  emitter.Run(addrs, err);
  if (err) {
    spdlog::error("Failed to emit: {}", err.message());
    return;
  }
}

void Receive(int device_index) {
  std::error_code err;
  Receiver receiver(device_index);
  receiver.Run(err);
  if (err) {
    spdlog::error("Failed to receive: {}", err.message());
    return;
  }
}

int main(int argc, char* argv[]) {
  cxxopts::Options options(argv[0], "OpenAC Cast");
  options.positional_help("[optional args]").show_positional_help();
  options.add_options()
      ("h,help", "Print help")
      ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"))
      ("l,list", "List available devices", cxxopts::value<bool>()->default_value("false"))
      ("d,device", "The device index", cxxopts::value<int>()->default_value("-1"))
      ("e,emit", "Emit", cxxopts::value<bool>()->default_value("false"))
      ("r,receive", "Receive", cxxopts::value<bool>()->default_value("false"))
      ("addr", "Addresses", cxxopts::value<std::vector<std::string>>()->default_value("127.0.0.1"));

  auto result = options.parse(argc, argv);
  if (result.count("help")) {
    std::cout << options.help({"", "Group"}) << std::endl;
    return 0;
  }

  if (result["verbose"].as<bool>()) {
    spdlog::set_level(spdlog::level::debug);
  }
  pa::Context::Initialize();

  if (result["list"].as<bool>()) {
    ListDevices();
    return 0;
  }

  if (result["emit"].as<bool>()) {
    Emit(result["device"].as<int>(), result["addr"].as<std::vector<std::string>>());
  }

  if (result["receive"].as<bool>()) {
    Receive(result["device"].as<int>());
  }

  return 0;
}
