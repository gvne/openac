set(dntp_test_sources
  dntp_test.cc
)
add_executable(dntp_test ${dntp_test_sources})
target_link_libraries(dntp_test
  oac::dntp
  gtest
  gtest_main
)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${dntp_test_sources})
set_target_properties(dntp_test PROPERTIES FOLDER test)
