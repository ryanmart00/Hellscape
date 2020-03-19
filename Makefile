
CXX = g++
CXX_FLAGS = -Wall -std=c++1z

VCPKG = /home/ryan/Documents/vcpkg/packages

INC = -I./include -I$(VCPKG)/bullet3_x64-linux/include/bullet \
-I$(VCPKG)/glad_x64-linux/include/ \
-I$(VCPKG)/glm_x64-linux/include/ \
-I./libs/glfw/include/ \
-I./libs/assimp-build/include


LIBS = -L$(VCPKG)/bullet3_x64-linux/lib/ \
-L./libs/glfw-build/src \
-L$(VCPKG)/glad_x64-linux/lib/ \
-L./libs/assimp-build/code/ \
-lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath\
-lglfw3 -lglad -lassimp -lGL -lX11 -lpthread -lXrandr -lXi -ldl 

includes = $(wildcard include/*.hpp)

DEBUG = 0

all: main

main: main.o camera.o shader.o mesh.o model.o dynamics.o
	$(CXX) -o main *.o $(LIBS)

main.o: src/main.cpp ${includes}
	$(CXX) $(CXX_FLAGS) -c $(INC) src/main.cpp -DDEBUG=$(DEBUG)

camera.o: src/camera.cpp include/camera.hpp
	$(CXX) $(CXX_FLAGS) -c $(INC) src/camera.cpp

shader.o: src/shader.cpp include/shader.hpp
	$(CXX) $(CXX_FLAGS) -c $(INC) src/shader.cpp

mesh.o: src/mesh.cpp include/mesh.hpp include/shader.hpp
	$(CXX) $(CXX_FLAGS) -c $(INC) src/mesh.cpp

model.o: src/model.cpp include/model.hpp include/shader.hpp include/mesh.hpp
	$(CXX) $(CXX_FLAGS) -c $(INC) src/model.cpp

dynamics.o: src/dynamics.cpp include/dynamics.hpp include/debug_drawer.hpp
	$(CXX) $(CXX_FLAGS) -c $(INC) src/dynamics.cpp
	

clean: 
	rm main *.o
