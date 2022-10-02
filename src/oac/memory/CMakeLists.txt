set(memory_sources
  circular_buffer.h

  sliding_window.h
  sliding_window.cc
)
add_library(oac_memory ${memory_sources})

target_link_libraries(oac_memory PUBLIC
  spdlog
)

target_include_directories(oac_memory PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../..>
  $<INSTALL_INTERFACE:include>
)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${memory_sources})
set_target_properties(oac_memory PROPERTIES FOLDER src/oac)

add_library(oac::memory ALIAS oac_memory)
