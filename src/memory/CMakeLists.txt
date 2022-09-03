set(memory_sources
  endian.h
  endian.cc
  circular_buffer.h
)
add_library(memory ${memory_sources})
target_include_directories(memory PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  $<INSTALL_INTERFACE:include>
)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${memory_sources})
set_target_properties(memory PROPERTIES FOLDER src)

include (TestBigEndian)
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
if(IS_BIG_ENDIAN)
  target_compile_definitions(memory PUBLIC IS_BIG_ENDIAN)
else()
  target_compile_definitions(memory PUBLIC IS_LITTLE_ENDIAN)
endif()
