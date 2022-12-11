set(resample_sources
  lagrange.h
  lagrange.cc
  interpolator.h
)

add_library(resample ${resample_sources})

target_include_directories(resample PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  $<INSTALL_INTERFACE:include>
)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${resample_sources})
set_target_properties(resample PROPERTIES FOLDER src)
