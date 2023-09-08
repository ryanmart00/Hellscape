# Hellscape

## Pre-built Binaries
<a id="raw-url" href="https://github.com/ryanmart00/Hellscape/raw/master/builds/Win64/Hellscape.zip"> Windows 64-bit</a>

<a id="raw-url" href="https://github.com/ryanmart00/Hellscape/raw/master/builds/Linux/Hellscape.zip"> Linux 64-bit</a>

## Or:
Build from source with cmake

### Instructions:

On Linux:
- make a bin 
- run cmake /path/to/Hellscape/
- run make
- may need libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl-dev

On Windows:
- Install MSYS2 (for example): https://www.msys2.org/ Avoid installing MSYS2 in a folder with spaces in its name! (This will cause problems!)
- In a MSYS2 window clone repo with --recurse-submodules to pull required libraries ```git clone --recurse-submodules https://github.com/ryanmart00/Hellscape.git```
- You'll need the following packages: ```pacman -S cmake make gcc```
- You will probably also need some graphics drivers, but this is system dependent. To search for drivers run ```pacman -Ss mesa``` and use pacman -S with the exact name of the one that matches your system
- after moving into the Hellscape folder run ```mkdir bin && cd bin```
- To compile the code: ```cmake -G "Unix Makefiles" .. -DCMAKE_SYSTEM_NAME=Windows && make```

Installation Notes
- This project depends on linked github repos, be sure to clone like `git clone --recurse-submodules ...`
- Rough set of dependencies: OpenGL, Cpp compiler (g++, clang), glfw (glfw is also linked repo, so you may not need to install this seperately)


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
