cmake_minimum_required(VERSION 3.5.0)
project(SmoothieEditor)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES 

"${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" 
"${CMAKE_SOURCE_DIR}/inc/imgui/imgui.h" "${CMAKE_SOURCE_DIR}/inc/imgui/imgui.cpp" 
"${CMAKE_SOURCE_DIR}/inc/imgui/backends/imgui_impl_glfw.h" "${CMAKE_SOURCE_DIR}/inc/imgui/backends/imgui_impl_glfw.cpp"
"${CMAKE_SOURCE_DIR}/inc/imgui/backends/imgui_impl_opengl3.h" "${CMAKE_SOURCE_DIR}/inc/imgui/backends/imgui_impl_opengl3.cpp"
"${CMAKE_SOURCE_DIR}/inc/imgui/imgui_demo.cpp" "${CMAKE_SOURCE_DIR}/inc/imgui/imgui_draw.cpp" 
"${CMAKE_SOURCE_DIR}/inc/imgui/imgui_tables.cpp" "${CMAKE_SOURCE_DIR}/inc/imgui/imgui_widgets.cpp" 
)

add_executable(SmoothieEditor ${SOURCES})

set(CXXFLAGS "${CXXFLAGS} /permissive-")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive-")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /permissive-")


#Include directories
target_include_directories(SmoothieEditor PRIVATE ${CMAKE_SOURCE_DIR}/SmoothieEngine/Core)
target_include_directories(SmoothieEditor PRIVATE ${CMAKE_SOURCE_DIR}/SmoothieEngine/Math)
target_include_directories(SmoothieEditor PRIVATE ${CMAKE_SOURCE_DIR}/SmoothieEngine/ResourceManager)
target_include_directories(SmoothieEditor PRIVATE ${CMAKE_SOURCE_DIR}/inc/GLFW/include)
target_include_directories(SmoothieEditor PRIVATE ${CMAKE_SOURCE_DIR}/inc/imgui)
target_include_directories(SmoothieEditor PRIVATE ${CMAKE_SOURCE_DIR}/inc/imgui/backends)
target_include_directories(SmoothieEditor PRIVATE ${CMAKE_SOURCE_DIR}/inc/imgui/examples/libs/glfw/include)


target_link_libraries(SmoothieEditor PRIVATE  ${CMAKE_SOURCE_DIR}/lib/Debug/SmoothieEngine.lib)
target_link_libraries(SmoothieEditor PRIVATE  ${CMAKE_SOURCE_DIR}/inc/GLFW/lib/glfw3.lib)
target_link_libraries(SmoothieEditor PRIVATE opengl32.lib)


#WILL BE REMOVED
target_link_libraries(SmoothieEditor PRIVATE  ${CMAKE_SOURCE_DIR}/inc/GLAD/lib/glew32s.lib)
target_compile_definitions(SmoothieEditor PRIVATE GLEW_STATIC)

link_libraries()
set_target_properties(
    SmoothieEditor PROPERTIES

    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/Demo
)
