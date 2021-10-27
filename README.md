Vulkan Demo

Prerequisites <br>
-- git <br>
-- cmake <br>
-- vulkan

Env <br>
-- VULKAN_SDK (required) <br>
-- GLFW_HOME (optional)

Setup

1 submodules <br>
git submodule update --init

2 glfw <br>
if use pre-installed binaries <br>
-- set glfw location env

if compile and install binaries <br>
-- run cmake (script) <br>
-- set glfw location env

if build source <br>
-- _no action required_

3 generate project files <br>
-- run cmake (script) <br>
-- !! visual studio solution ?? <br>
-- !! xcode project ?? <br>

4 compile and install project <br>
-- run cmake (script) <br>
-- !! shaders --> glslc --> spv <br>
-- !! visual studio settings ?? <br>
-- !! xcode project settings ?? <br>