set(dntp_sources
  message.h
  message.cc
  timestamp.h
  timestamp.cc

  server_interface.h
  server.h
  server.cc
  client.h
  client.cc
)
add_library(dntp ${dntp_sources})
target_link_libraries(dntp
  memory

  asio
  spdlog
)
target_include_directories(dntp PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  $<INSTALL_INTERFACE:include>
)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${dntp_sources})
set_target_properties(dntp PROPERTIES FOLDER src)
