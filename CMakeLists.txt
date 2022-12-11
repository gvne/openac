cmake_minimum_required(VERSION 3.11)

project(openac VERSION 0.1)

set(openac_is_toplevel_project FALSE)
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(openac_is_toplevel_project TRUE)
endif()

# Avoid showing external dependencies at top level on Xcode
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD 20)
option(openac_enable_tests "Enable tests" ${openac_is_toplevel_project})
option(openac_enable_juce_tests "Enable JUCE tests" ${openac_is_toplevel_project})

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/add_asio.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/add_comms.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/add_spdlog.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/add_structutils.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/add_portaudio.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/add_cxxopts.cmake)

add_subdirectory(src)
if(${openac_enable_tests})
  enable_testing()
  include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/add_googletest.cmake)
  add_subdirectory(test)
endif()
