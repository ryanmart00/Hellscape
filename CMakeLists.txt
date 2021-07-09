cmake_minimum_required(VERSION 3.10)

# set the project name
project(PushGame)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

#include header files
include_directories(include)

if(WIN32)
    message("WIN32")
    file(GLOB WINDOWSDLL "libs/WindowsDll/*")
    file(COPY ${WINDOWSDLL} DESTINATION ${CMAKE_BINARY_DIR})
    set(CMAKE_CXX_FLAGS "-D_GNU_SOURCE -Wa,-mbig-obj -O1")
    set(CMAKE_C_FLAGS "-D_GNU_SOURCE -Wa,-mbig-obj -O1")
    set(ENABLE_PRECOMPILED_HEADERS OFF)
    add_definitions(-DGCC_WINDRES)
    set(WIN_LINKS "gdi32")
endif(WIN32)



#include assimp
#---------------------------------
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Do not build tests")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Do not build ANY shared libs")
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Do not build tools")
add_subdirectory(libs/assimp)

include_directories(libs/assimp/include)
link_directories(${PROJECT_BINARY_DIR}/libs/assimp/code)

#include glm (header only library)
#---------------------------------
include_directories(libs/glm)

#include bullet
#---------------------------------
set(BUILD_CPU_DEMOS OFF CACHE BOOL "Do not build cpu demos")
set(BUILD_OPENGL3_DEMOS OFF CACHE BOOL "Do not build opengl demos")
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "Do not build bullet 2 demos")
set(BUILD_EXTRAS OFF CACHE BOOL "Do not build extras")
set(INSTALL_LIBS OFF CACHE BOOL "Do not install libs")
set(BUILD_UNIT_TESTS OFF CACHE BOOL "Do not build tests")
add_subdirectory(libs/bullet3)

#write a MACRO to get files (Bullet lame-ly doesn't group includes)
MACRO(MATCH_DIRECTORIES glob return_list)
    FILE(GLOB_RECURSE new_list ${glob})
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

MATCH_DIRECTORIES(libs/bullet3/src/*.h* BULLET_INCLUDE)
FILE(GLOB BULLET_LINK ${PROJECT_BINARY_DIR}/libs/bullet3/src/**)
message("Bullet link" ${BULLET_LINK})
include_directories(${BULLET_INCLUDE})
include_directories(libs/bullet3)
link_directories(${BULLET_LINK})


#include glfw
#---------------------------------
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Do not build GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Do not build GLFW tests")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Do not build GLFW Docs")
set(GLFW_INSTALL OFF CACHE BOOL "Do not install GLFW")
add_subdirectory(libs/glfw)
include_directories(libs/glfw/include)
link_directories(${PROJECT_BINARY_DIR}/libs/glfw/src)


#include freetype
#---------------------------------
add_subdirectory(libs/freetype)
include_directories(libs/freetype/include)
link_directories(${PROJECT_BINARY_DIR}/libs/freetype/builds)

#set SOURCES to all files in src ending in .cpp
file(GLOB SOURCES "src/*.c*")


# add the executable
add_executable(PushGame ${SOURCES})

option(DEBUG "Will print Debug information to std out" OFF)
if(${DEBUG})
    message("Compiling with debug info")
    set_target_properties(PushGame PROPERTIES COMPILE_DEFINITIONS "DEBUG")
    target_compile_options(PushGame PRIVATE -g)
endif(${DEBUG})

target_link_libraries(PushGame 
    BulletSoftBody BulletDynamics BulletCollision LinearMath 
    assimp 
    glfw ${GLFW_LIBRARIES}
    ${WIN_LINKS} freetype ${FREETYPE_LIBRARIES})
target_include_directories(PushGame PRIVATE ${FREETYPE_INCLUDE_DIRS})

#-lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath\
#-lglfw3 -lglad -lassimp -lGL -lX11 -lpthread -lXrandr -lXi -ldl 

#add zipping commands
option(ZIP "Zip the files into the builds folder" OFF)
if(${ZIP} AND WIN32)
    message("Zipping for windows")
    add_custom_command(TARGET PushGame 
		POST_BUILD
		COMMAND zip 
		ARGS -r ${CMAKE_SOURCE_DIR}/builds/Win64/PushGame.zip 
		PushGame.exe msys-2.0.dll msys-gcc_s-seh-1.dll msys-stdc++-6.dll
		libwinpthread-1.dll zlib1.dll assets)
elseif(${ZIP})
    message("Zipping for linux")
    add_custom_command(TARGET PushGame 
		POST_BUILD
		COMMAND zip 
		ARGS -r ${CMAKE_SOURCE_DIR}/builds/Linux/PushGame.zip 
		PushGame assets)
endif()

