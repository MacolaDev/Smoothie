cmake_minimum_required(VERSION 3.5.0)

project(Exporter)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
add_library(Exporter SHARED ${SOURCES})


find_package(Python REQUIRED COMPONENTS Interpreter Development)

target_include_directories(Exporter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(Exporter PRIVATE ${Python_INCLUDE_DIRS})
target_link_libraries(Exporter PRIVATE ${Python_LIBRARIES})

#TODO: Update this for linux
set_target_properties(Exporter PROPERTIES SUFFIX ".pyd")

#Name of a function must be the same as name in Init function inside module
set_target_properties(Exporter PROPERTIES OUTPUT_NAME "SmoothieExporter")

link_libraries()
set_target_properties( Exporter PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/Exporter
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/Exporter
)