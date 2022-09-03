juce_add_console_app(wire_emit PRODUCT_NAME "OAC test wire Emit")
set(wire_emit_sources emit.cc)
target_sources(wire_emit PRIVATE ${wire_emit_sources})
juce_generate_juce_header(wire_emit)
target_compile_definitions(wire_emit PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
)
target_link_libraries(wire_emit PRIVATE
  wire
  juce::juce_core
  juce::juce_audio_utils
  asio
  spdlog
)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${wire_emit_sources})
set_target_properties(wire_emit PROPERTIES FOLDER test/juce/wire)

juce_add_console_app(wire_receive PRODUCT_NAME "OAC test wire Receive")
set(wire_receive_sources receive.cc)
target_sources(wire_receive PRIVATE ${wire_receive_sources})
juce_generate_juce_header(wire_receive)
target_compile_definitions(wire_receive PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
)
target_link_libraries(wire_receive PRIVATE
  wire
  juce::juce_core
  juce::juce_audio_utils
  asio
  spdlog
)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${wire_receive_sources})
set_target_properties(wire_receive PROPERTIES FOLDER test/juce/wire)

if (MSVC)
  target_compile_definitions(wire_emit PRIVATE WIN)
  target_compile_definitions(wire_receive PRIVATE WIN)
endif()
