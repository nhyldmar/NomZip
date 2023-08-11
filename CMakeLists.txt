cmake_minimum_required(VERSION 3.1)

project(NomZip)

set(CMAKE_CXX_STANDARD 14)
set(EXECUTABLE_OUTPUT_PATH bin/)
set(LIBRARY_OUTPUT_PATH lib/)

include_directories(lib/cxxopts/include)

include_directories(lib/nomzip/include)
add_library(nomzip lib/nomzip/src/ProbabilityModelGenerator.cpp)

add_executable(zip src/zip.cpp)
target_link_libraries(zip nomzip)

add_executable(generate src/generate.cpp)
target_link_libraries(generate nomzip)

add_executable(test tests/test.cpp)
target_link_libraries(test nomzip)
