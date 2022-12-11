include(FetchContent)

FetchContent_Declare(portaudio
  GIT_REPOSITORY https://github.com/PortAudio/portaudio.git
  GIT_TAG v19.7.0
)
FetchContent_GetProperties(portaudio)
if(NOT portaudio_POPULATED)
  FetchContent_Populate(portaudio)
  add_subdirectory(${portaudio_SOURCE_DIR} ${portaudio_BINARY_DIR})
endif()

set_target_properties(
  portaudio
  PROPERTIES FOLDER deps/portaudio
)

set_target_properties(
  portaudio_static
  PROPERTIES FOLDER deps/portaudio
)
