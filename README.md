# Vulkan Tutorial

<div float="left">
  <img src="./img/vulkan-tutorial.gif" width="800" />
</div>

# Description

My implementation of [Vulkan Tutorial][vulkantutorial] where you create an application that can load and render a 3D model using the Vulkan graphics API and C++.

# Resources&nbsp;:books:

- [Vulkan Tutorial][vulkantutorial] (Website)
- [Vulkan Guide][vkguide] (Website)
- [Brendan Galea: Vulkan Game Engine Tutorial][brendangalea:vulkantutorial] (YouTube @ [Brendan Galea][brendangalea])
- [Vulkan C++ examples and demos][saschawillems:vulkan] (Github @ [Sascha Willems][saschawillems])
- [The Cherno: C++][thecherno:cpp] (YouTube @ [The Cherno][thecherno])

# Prerequisites&nbsp;:vertical_traffic_light:

- [Git][git]
- [CMake][cmake]
- [Vulkan][vulkan]
- C++ compiler
    - Windows: [MSVC][msvc] (Bundled with [Visual Studio][msvs])
    - MacOS: [Clang][clang] (Bundled with [Xcode][xcode])
    - Linux: [GCC][gcc]

# Getting started&nbsp;:runner:

### Getting the code&nbsp;:octocat:

- Clone the repository: `git clone https://github.com/taardal/vulkan-tutorial`

### Installing dependencies&nbsp;:floppy_disk:

- Clone submodules: `git submodule update --init`

### Building the app&nbsp;:building_construction:

- Generate build files: `cmake -D BUILD_GLFW_SRC=ON -S . -B build`
  - The `BUILD_GLFW_SRC` argument specifies that the [GLFW][glfw] library source code should be built as part of this project.
  - The `BUILD_GLFW_SRC` argument is not needed if [GLFW][glfw] is already installed on the system (See [Installing GLFW](#installing-glfw)).
- Build the binary: `cmake --build build`
  - Output folder: `./bin/debug`

### Running the app&nbsp;:rocket:

- Navigate to the output folder: `cd bin/debug`
- Run the executable: `./vulkandemo`

# Installing the CLI

This app also features its own CLI to make it easier to do things like re-compiling the shaders, adding dependencies as git submodules and installing GLFW to the system.

### Installing the CLI

**Script**

- Run the install-script: `./scripts/install_cli.sh`

**Manual**

- Navigate to the CLI folder: `cd cli`
- Generate build files: `cmake -S . -B build`
- Build the binary: `cmake --build build`
- Install the binary: `cmake --install build`
  - Installs the binary to parent project root

### Running the CLI

- Navigate back to parent project root: `cd ..`
- Run the CLI: `./vd`

```
NAME:
    vd - Vulkandemo CLI

USAGE:
    vd [global options] command [command options] [arguments...]

COMMANDS:
    build          Build project
    glfw           Install GLFW on local machine
    lib:add        Add library (git submodule)
    lib:install    Install libraries (git submodules)
    run, r         Run project
    shaders        Compile shaders
    help, h        Show available commands and options

GLOBAL OPTIONS:
    --help, -h    Show help
```

# Installing GLFW

The [GLFW][glfw] library can be installed on the system to decrease build times by not having to build it as part of building the app. 

### Installing the library

Docs: [Compiling GLFW][glfw:compile]

- Navigate to the glfw folder: `cd lib/glfw`
- Generate build files: `cmake -S . -B build`
- Build the library: `cmake --build build`
- Install the library: `cmake --install build`

Install destinations:
```
Mac/Linux
  Header: /usr/local/include/GLFW
  Binary: /usr/local/lib/cmake/glfw3

Windows
  x64: C:\Program Files\GLFW
  x86: C:\Program Files (x86)\GLFW
```


[brendangalea]: https://www.youtube.com/@BrendanGalea
[brendangalea:vulkantutorial]: https://www.youtube.com/watch?v=Y9U9IE0gVHA&list=PL8327DO66nu9qYVKLDmdLW_84-yE4auCR&ab_channel=BrendanGalea
[clang]: http://clang.org/
[cmake]: https://cmake.org/
[gcc]: https://gcc.gnu.org/
[git]: https://git-scm.com
[glfw]: https://www.glfw.org
[glfw:compile]: https://www.glfw.org/docs/3.3/compile.html
[msvc]: https://visualstudio.microsoft.com/vs/features/cplusplus/
[msvs]: https://visualstudio.microsoft.com/
[saschawillems]: https://github.com/SaschaWillems
[saschawillems:vulkan]: https://github.com/SaschaWillems/Vulkan
[thecherno]: https://www.youtube.com/@TheCherno
[thecherno:cpp]: https://www.youtube.com/watch?v=18c3MTX0PK0&list=PLlrATfBNZ98dudnM48yfGUldqGD0S4FFb&ab_channel=TheCherno
[vkguide]: https://vkguide.dev/
[vulkan]: https://vulkan.lunarg.com/
[vulkantutorial]: https://vulkan-tutorial.com/
[xcode]: https://developer.apple.com/xcode/