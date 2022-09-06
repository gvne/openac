set(memory_sources
  endian.h
  endian.cc
  circular_buffer.h
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

include (TestBigEndian)
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
if(IS_BIG_ENDIAN)
  target_compile_definitions(oac_memory PUBLIC IS_BIG_ENDIAN)
else()
  target_compile_definitions(oac_memory PUBLIC IS_LITTLE_ENDIAN)
endif()

add_library(oac::memory ALIAS oac_memory)
