cmake_minimum_required(VERSION 3.16)

project(Wav)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(wav
    src/wav.cpp
)

# Put the final executable in the project root
set_target_properties(wav PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)