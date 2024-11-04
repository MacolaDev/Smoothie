cmake_minimum_required(VERSION 3.5.0)

project(SmoothieEngine)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
add_library(SmoothieEngine STATIC ${SOURCES})

#3th party includes
target_include_directories(SmoothieEngine PRIVATE ${CMAKE_SOURCE_DIR}/inc/GLAD/include)
target_include_directories(SmoothieEngine PRIVATE ${CMAKE_SOURCE_DIR}/include/glm)
target_include_directories(SmoothieEngine PRIVATE ${CMAKE_SOURCE_DIR}/include/stb)
target_include_directories(SmoothieEngine PRIVATE ${CMAKE_SOURCE_DIR}/inc/GLFW/include)

target_include_directories(SmoothieEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Core)
target_include_directories(SmoothieEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Math)
target_include_directories(SmoothieEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ResourceManager)

#Link libraries
target_link_libraries(SmoothieEngine PRIVATE opengl32.lib)
target_link_libraries(SmoothieEngine PRIVATE  ${CMAKE_SOURCE_DIR}/inc/GLAD/lib/glew32s.lib)
target_link_libraries(SmoothieEngine PRIVATE  ${CMAKE_SOURCE_DIR}/inc/GLFW/lib/glfw3.lib)

#For static linking of glew32s.lib
target_compile_definitions(SmoothieEngine PRIVATE GLEW_STATIC)

set(CXXFLAGS "${CXXFLAGS} /permissive-")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive-")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /permissive-")
link_libraries()



set_target_properties(
    SmoothieEngine PROPERTIES

    ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib/Debug
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/Release

    OBJECT_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/intermediate/Debug"
    OBJECT_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/intermediate/Release"
)