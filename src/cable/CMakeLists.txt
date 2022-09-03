set(cable_sources
  publisher.h
  publisher.cc
  listener.h
  listener.cc
)
add_library(cable ${cable_sources})
target_link_libraries(cable
  memory
  dntp
  wire

  asio
  spdlog
)
target_include_directories(cable PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  $<INSTALL_INTERFACE:include>
)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${cable_sources})
set_target_properties(cable PROPERTIES FOLDER src)
