set(memory_test_sources
  memory_test.cc
)
add_executable(memory_test ${memory_test_sources})
target_link_libraries(memory_test
  memory
  gtest
  gtest_main
)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${memory_test_sources})
set_target_properties(memory_test PROPERTIES FOLDER test)
