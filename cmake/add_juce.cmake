include(FetchContent)

FetchContent_Declare(juce
  GIT_REPOSITORY https://github.com/juce-framework/JUCE
  GIT_TAG 7.0.2
)
FetchContent_GetProperties(juce)
if(NOT juce_POPULATED)
  FetchContent_Populate(juce)
  add_subdirectory(${juce_SOURCE_DIR} ${juce_BINARY_DIR})
endif()

set_target_properties(
  juce_lv2_helper
  PROPERTIES FOLDER deps/juce
)
