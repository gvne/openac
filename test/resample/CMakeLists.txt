set(resample_test_sources
  resample_test.cc
)
add_executable(resample_test ${resample_test_sources})
target_link_libraries(resample_test
  resample
  gtest
  gtest_main
)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${resample_test_sources})
set_target_properties(resample_test PROPERTIES FOLDER test)

add_test(NAME resample_test COMMAND resample_test)
