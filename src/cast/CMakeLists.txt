set(cast_sources
  main.cc

  emitter.h
  emitter.cc
  receiver.h
  receiver.cc
)
add_executable(cast ${cast_sources})
target_link_libraries(cast
  oac::cable
  resample
  portaudiopp

  asio
  spdlog
  cxxopts
)
target_include_directories(cast PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  $<INSTALL_INTERFACE:include>
)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${cast_sources})
set_target_properties(cast PROPERTIES FOLDER src)
