include(FetchContent)

FetchContent_Declare(structutils
  GIT_REPOSITORY https://github.com/gvne/structutils
  GIT_TAG 682b28874f62e20aba544f117d0a2fa09fbf036c
)
FetchContent_GetProperties(structutils)
if(NOT structutils_POPULATED)
  FetchContent_Populate(structutils)
  set(astruct_enable_tests "" CACHE BOOL OFF)
  add_subdirectory(${structutils_SOURCE_DIR} ${structutils_BINARY_DIR})
endif()
