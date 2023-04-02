# PushGame

## Pre-built Binaries
<a id="raw-url" href="https://github.com/ryanmart00/Hellscape/raw/master/builds/Win64/Hellscape.zip"> Windows 64-bit</a>

<a id="raw-url" href="https://github.com/ryanmart00/Hellscape/raw/master/builds/Linux/Hellscape.zip"> Linux 64-bit</a>

## Or:
Build from source with cmake

### Instructions:
- clone repo with --recurse-submodules to pull required libraries

On Linux:
- make a bin 
- run cmake /path/to/Hellscape/
- run make

On Windows:
- Install MSYS2 (for example): https://www.msys2.org/
- You'll need the following packages: pacman -S cmake make gcc 
- Run cmake "/path/to/Hellscape/" -DCMAKE_SYSTEM_NAME=Windows
- run make


## Concept Map:
<a id="raw-url" href="https://miro.com/app/board/o9J_knGxncc=/"> Miro</a>


## Credits:
This project uses the following packages according to each licence agreement. Links to their sites are listed below:

- [Assimp](https://github.com/assimp/assimp)
- [Bullet3](https://github.com/bulletphysics/bullet3)
- [Freetype](https://www.freetype.org/index.html)
- [Glad](https://github.com/Dav1dde/glad)
- [GLFW](https://github.com/glfw/glfw)
- [GLM](https://github.com/g-truc/glm)
