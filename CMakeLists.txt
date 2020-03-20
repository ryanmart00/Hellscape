cmake_minimum_required(VERSION 3.10)

# set the project name
project(HellScape)

#include header files
include_directories(include)

#include glm (header only library)
#---------------------------------
include_directories(libs/glm)

#include hacky bullet
#---------------------------------
include_directories(/usr/local/include/bullet)

#include glfw
#---------------------------------
add_subdirectory(libs/glfw)
include_directories(${MyProject_SOURCE_DIR}/libs/glfw/include)
link_directories(${MyProject_SOURCE_DIR}/libs/glfw/src)

#include assimp
#---------------------------------
add_subdirectory(libs/assimp)
include_directories(${MyProject_SOURCE_DIR}/libs/assimp/include)
link_directories(${MyProject_SOURCE_DIR}/libs/assimp/src)

#set SOURCES to all files in src ending in .cpp
file(GLOB SOURCES "src/*.cpp")

# add the executable
add_executable(HellScape ${SOURCES})
target_link_libraries(HellScape GLFW bullet3 ASSIMP)
