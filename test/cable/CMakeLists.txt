set(cable_test_sources
  cable_test.cc
)
add_executable(cable_test ${cable_test_sources})
target_link_libraries(cable_test
  oac::cable
  gtest
  gtest_main
)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${cable_test_sources})
set_target_properties(cable_test PROPERTIES FOLDER test)
