cmake_minimum_required(VERSION 3.5.0)

project(Smoothie)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
find_package(Python3 REQUIRED)
find_package(PythonLibs REQUIRED)

add_subdirectory(SmoothieEngine)
add_subdirectory(Demo)
add_subdirectory(Exporter)
add_subdirectory(SmoothieEditor)

add_dependencies(Demo SmoothieEngine)
add_dependencies(SmoothieEditor SmoothieEngine)

link_libraries()