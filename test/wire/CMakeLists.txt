set(wire_test_sources
  wire_test.cc
)
add_executable(wire_test ${wire_test_sources})
target_link_libraries(wire_test
  oac::wire
  gtest
  gtest_main
)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${wire_test_sources})
set_target_properties(wire_test PROPERTIES FOLDER test)
