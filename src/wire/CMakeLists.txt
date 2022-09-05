set(wire_sources
  message.h
  message.cc

  publisher_interface.h
  publisher.h
  publisher.cc

  listener.h
  listener.cc
  listener/output.h
  listener/buffer.h
  listener/buffer.cc
)
add_library(wire ${wire_sources})
target_link_libraries(wire
  memory
  dntp

  asio
  spdlog
  astruct
)
target_include_directories(wire PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  $<INSTALL_INTERFACE:include>
)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${wire_sources})
set_target_properties(wire PROPERTIES FOLDER src)
