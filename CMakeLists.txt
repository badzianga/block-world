cmake_minimum_required(VERSION 4.0)
project(MineCppraft)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(MineCppraft main.cpp)
target_link_libraries(MineCppraft PRIVATE GL GLEW glfw)
