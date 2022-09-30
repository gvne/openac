set(dntp_sources
  message.h
  message.cc
  #
  # utils.h
  # utils.cc
  #
  server_interface.h
  server.h
  server.cc
  client.h
  client.cc
)
add_library(oac_dntp ${dntp_sources})
target_link_libraries(oac_dntp
  oac::memory
  oac::chrono

  asio
  oac_msg
  comms
  spdlog
)
target_include_directories(oac_dntp PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
  $<INSTALL_INTERFACE:include>
)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${dntp_sources})
set_target_properties(oac_dntp PROPERTIES FOLDER src/oac)

add_library(oac::dntp ALIAS oac_dntp)
