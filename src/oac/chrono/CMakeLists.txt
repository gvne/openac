set(chrono_sources
  local_clock.h
  local_clock.cc
  stream_clock.h
  stream_clock.cc
)
add_library(oac_chrono ${chrono_sources})
target_link_libraries(oac_chrono
  spdlog
)
target_include_directories(oac_chrono PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
  $<INSTALL_INTERFACE:include>
)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${chrono_sources})
set_target_properties(oac_chrono PROPERTIES FOLDER src/oac)

add_library(oac::chrono ALIAS oac_chrono)
