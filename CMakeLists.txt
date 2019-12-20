cmake_minimum_required(VERSION 3.1)

project(NicZip)

set(CMAKE_CXX_STANDARD 14)
set(EXECUTABLE_OUTPUT_PATH bin/)
set(LIBRARY_OUTPUT_PATH lib/)

include_directories(lib/cxxopts/include)
include_directories(lib/json/include)

include_directories(lib/niczip/include)
add_library(niczip lib/niczip/src/ProbabilityModelGenerator.cpp)

add_executable(zip src/zip.cpp)
target_link_libraries(zip niczip)

add_executable(generate src/generate.cpp)
target_link_libraries(generate niczip)

add_executable(test tests/test.cpp)
