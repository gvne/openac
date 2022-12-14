juce_add_console_app(cable_emit PRODUCT_NAME "OAC test cable Emit")
set(cable_emit_sources
  emit.cc
  init.h
  init.cc
  input_app_component.cc
  input_app_component.h
)
target_sources(cable_emit PRIVATE ${cable_emit_sources})
juce_generate_juce_header(cable_emit)
target_compile_definitions(cable_emit PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
)
target_link_libraries(cable_emit PRIVATE
  oac::cable
  juce::juce_core
  juce::juce_audio_utils
  juce::juce_audio_basics
  asio
  spdlog
)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${cable_emit_sources})
set_target_properties(cable_emit PROPERTIES FOLDER test/juce/cable)

juce_add_console_app(cable_receive PRODUCT_NAME "OAC test cable Receive")
set(cable_receive_sources
  receive.cc
  init.h
  init.cc
  output_app_component.cc
  output_app_component.h
)
target_sources(cable_receive PRIVATE ${cable_receive_sources})
juce_generate_juce_header(cable_receive)
target_compile_definitions(cable_receive PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
)
target_link_libraries(cable_receive PRIVATE
  oac::cable
  juce::juce_core
  juce::juce_audio_utils
  juce::juce_audio_basics
  asio
  spdlog
)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${cable_receive_sources})
set_target_properties(cable_receive PROPERTIES FOLDER test/juce/cable)

if (MSVC)
  target_compile_definitions(cable_emit PRIVATE WIN)
  target_compile_definitions(cable_receive PRIVATE WIN)
endif()
