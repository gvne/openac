set(portaudiopp_sources
  portaudiopp.h

  context.h
  context.cc
  device.h
  device.cc
  error.h
  error.cc
  stream.h
  stream.cc
)

add_library(portaudiopp ${portaudiopp_sources})

set(dependencies
  spdlog
)
if (${BUILD_SHARED_LIBS})
  set(dependencies ${dependencies} portaudio)
else()
  set(dependencies ${dependencies} portaudio_static)
endif()

target_link_libraries(portaudiopp ${dependencies})

target_include_directories(portaudiopp PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
  $<INSTALL_INTERFACE:include>
)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${portaudiopp_sources})
set_target_properties(portaudiopp PROPERTIES FOLDER src)
