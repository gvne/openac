add_subdirectory(memory)
add_subdirectory(dntp)
add_subdirectory(wire)
add_subdirectory(cable)

if (${openac_enable_juce_tests})
  add_subdirectory(juce)
endif()
