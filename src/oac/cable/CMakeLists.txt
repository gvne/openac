set(cable_sources
  publisher.h
  publisher.cc
  listener.h
  listener.cc
)
add_library(oac_cable ${cable_sources})
target_link_libraries(oac_cable
  oac::memory
  oac::dntp
  oac::wire

  asio
  spdlog
)
target_include_directories(oac_cable PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
  $<INSTALL_INTERFACE:include>
)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${cable_sources})
set_target_properties(oac_cable PROPERTIES FOLDER src/oac)

add_library(oac::cable ALIAS oac_cable)
