cmake_minimum_required(VERSION 3.5.0)
project(Demo)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Demo "main.cpp" "Demo.cpp" "Demo.h")

set(CXXFLAGS "${CXXFLAGS} /permissive-")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive-")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /permissive-")

#Include directories
target_include_directories(Demo PRIVATE ${CMAKE_SOURCE_DIR}/SmoothieEngine/Core)
target_include_directories(Demo PRIVATE ${CMAKE_SOURCE_DIR}/SmoothieEngine/Math)
target_include_directories(Demo PRIVATE ${CMAKE_SOURCE_DIR}/SmoothieEngine/ResourceManager)
target_include_directories(Demo PRIVATE ${CMAKE_SOURCE_DIR}/inc/GLFW/include)

target_link_libraries(Demo PRIVATE  ${CMAKE_SOURCE_DIR}/lib/Debug/SmoothieEngine.lib)
target_link_libraries(Demo PRIVATE  ${CMAKE_SOURCE_DIR}/inc/GLFW/lib/glfw3.lib)
target_link_libraries(Demo PRIVATE opengl32.lib) #Will see if its going to be removed

#WILL BE REMOVED
target_include_directories(Demo PRIVATE ${CMAKE_SOURCE_DIR}/inc/GLAD/include)

#WILL BE REMOVED
target_include_directories(Demo PRIVATE ${CMAKE_SOURCE_DIR}/inc/glm)


#WILL BE REMOVED
target_link_libraries(Demo PRIVATE  ${CMAKE_SOURCE_DIR}/inc/GLAD/lib/glew32s.lib)
target_compile_definitions(Demo PRIVATE GLEW_STATIC)

link_libraries()
set_target_properties(
    Demo PROPERTIES

    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/Demo
)
