$vulkanPath = $ENV:VULKAN_SDK
$glslcPath = "${vulkanPath}/Bin/glslc"
$shaderPath = "res/shaders"
$shaderOutputPath = "cmake-build-debug/Debug/shaders"

$shaderOutputPathExists = Test-Path -Path $shaderOutputPath
if ($shaderOutputPathExists)
{
    Remove-Item $shaderOutputPath -Recurse
}
mkdir $shaderOutputPath

& ${glslcPath} "${shaderPath}/simple_shader.vert" -o "${shaderOutputPath}/simple_shader.vert.spv"
& ${glslcPath} "${shaderPath}/simple_shader.frag" -o "${shaderOutputPath}/simple_shader.frag.spv"
