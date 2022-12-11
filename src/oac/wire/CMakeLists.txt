set(wire_sources
  message.h
  message.cc
  payload_type.h
  payload_type.cc

  publisher_interface.h
  publisher.h
  publisher.cc

  listener.h
  listener.cc
  listener/output.h
  listener/buffer.h
  listener/buffer.cc
)
add_library(oac_wire ${wire_sources})
target_link_libraries(oac_wire
  oac::memory
  oac::dntp
  oac_msg
  comms

  asio
  spdlog
)
target_include_directories(oac_wire PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
  $<INSTALL_INTERFACE:include>
)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${wire_sources})
set_target_properties(oac_wire PROPERTIES FOLDER src/oac)

add_library(oac::wire ALIAS oac_wire)
