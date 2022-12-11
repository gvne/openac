include(FetchContent)

FetchContent_Declare(comms
  GIT_REPOSITORY https://github.com/commschamp/comms.git
  GIT_TAG v5.0.1
)

FetchContent_GetProperties(comms)
if(NOT comms_POPULATED)
  FetchContent_Populate(comms)
  add_library(comms INTERFACE)
  target_include_directories(comms INTERFACE
    $<BUILD_INTERFACE:${comms_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
  )
  set(OPT_REQUIRE_COMMS_LIB OFF CACHE BOOL "" FORCE)
endif()
