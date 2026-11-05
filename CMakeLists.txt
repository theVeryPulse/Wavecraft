cmake_minimum_required(VERSION 3.16)

project(Wav LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)

add_executable(wav_generator
    src/main.cpp
    src/SineOscillator.cpp
    src/WavHeader.cpp
)

add_executable(wav_player
    src/wav_player.cpp
)
target_link_libraries(wav_player PRIVATE PkgConfig::SDL2)

add_executable(cursor_pitch_volume
    src/cursor_pitch_volume.cpp
)
target_link_libraries(cursor_pitch_volume PRIVATE PkgConfig::SDL2)
