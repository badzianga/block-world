cmake_minimum_required(VERSION 4.0)
project(BlockWorld)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
        ${CMAKE_SOURCE_DIR}/include/*/*.hpp
        ${CMAKE_SOURCE_DIR}/include/*/*.inl
        ${CMAKE_SOURCE_DIR}/src/*/*.cpp
)

add_executable(BlockWorld main.cpp ${SRC_FILES})
target_include_directories(BlockWorld PRIVATE include vendor)
target_link_libraries(BlockWorld PRIVATE GL GLEW glfw glm)
