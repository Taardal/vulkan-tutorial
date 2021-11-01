# Vulkan Demo

## Prerequisites <br>
- [Git](https://git-scm.com/downloads)
- [CMake](https://cmake.org/download/)
- [Vulkan](https://vulkan.lunarg.com/)

## Environment variables
- `VULKAN_SDK`: Path to installed Vulkan SDK directory
  - Automatically added when installing Vulkan SDK
- `GLFW_HOME`: Path to installed GLFW directory
  - Manually added during setup. See --GETTING_STARTED_LINK--.

## Getting started
Use the scripts under `<project>/scripts/setup` to get started

### 1) Install dependencies
Run script:
```
./1-install_dependencies.sh
```
This project uses _git submodules_ for dependency management. 
The script will install all git submodules and detach them to specific versions specified in the script file.

[Troubleshooting](#troubleshooting)

### 2) Build and install GLFW
Run script:
```
./2-build_and_install_glfw.sh
```
The script will do three things:
1. Use CMake to generate project files for GLFW
2. Build the GLFW library using the generated files
3. Install the GLFW library to your computer
  - Windows 
    - x64: `C:\Program Files\GLFW`
    - x86: `C:\Program Files (x86)\GLFW`
  - Mac/Linux
    - Header: `/usr/local/include/GLFW`
    - Library: `/usr/local/lib/cmake/glfw3`

-- Explain arguments<br>
-- Explain selects/prompts<br>
-- !! BUILD_GLFW_SRC OPTION !!

#### Windows
- Add the `GLFW_HOME` environment variable with the path to the GLFW directory

[Troubleshooting](#troubleshooting)

### 3) Build project
```
./3-build_project.sh
```
-- Explain arguments<br>
-- Explain selects/prompts

[Troubleshooting](#troubleshooting)

### Troubleshooting

#### Windows:

#### Mac/Linux:

##### Shell: Permission denied
```
bash: permission denied: ./<script>.sh
```
Solution 1: Grant write-permission to file
```
chmod +x <script>.sh
```
Solution 2: Run script as admin
```
sudo ./<script>.sh
```

##### CMake: Permission denied
```
CMake Error: Could not open file for write in copy operation /path/to/some/file
CMake Error: : System Error: Permission denied
```
Solution 1: Run script as admin
```
sudo ./<script>.sh
```