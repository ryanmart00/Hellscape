cmake_minimum_required(VERSION 3.10)

# set the project name
project(HellScape)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

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
include_directories(libs/glfw/include)
link_directories(libs/glfw/src)

#set SOURCES to all files in src ending in .cpp
file(GLOB SOURCES "src/*.c*")

# add the executable
add_executable(HellScape ${SOURCES})
target_link_libraries(HellScape BulletSoftBody BulletDynamics BulletCollision
    LinearMath glfw3.a assimp GL X11 pthread Xrandr Xi dl)
#-lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath\
#-lglfw3 -lglad -lassimp -lGL -lX11 -lpthread -lXrandr -lXi -ldl 
