#!/bin/bash

readonly DEBUG_BUILD_TYPE="Debug"
readonly RELEASE_BUILD_TYPE="Release"

echo "
#####################
#   Setting up...   #
#####################
"
if [[ -n "${1}" ]]; then
  buildType="${1}"
else
  buildType="${DEBUG_BUILD_TYPE}"
fi

if [[ ${buildType} != "${DEBUG_BUILD_TYPE}" && ${buildType} != "${RELEASE_BUILD_TYPE}" ]]; then
  echo "Invalid build type [${buildType}], expected [${DEBUG_BUILD_TYPE},${RELEASE_BUILD_TYPE}]"
  exit 1
fi

echo "-- Using build type [${buildType}]"

buildDirectory="cmake-build-$(echo ${buildType} | awk '{print tolower($0)}')"
echo "-- Using build directory [${buildDirectory}]"

workingDirectory="$(pwd)"
echo "-- Working from directory [${workingDirectory}]"

glslcExe="$(VULKAN_SDK)/Bin/glslc"
echo "-- Using glslc exe [${glslcExe}]"

shaderDirectory="res/shaders"
echo "-- Using shader directory [${shaderDirectory}]"

shaderOutputDirectory="${buildDirectory}/shaders"
echo "-- Using shader output directory [${shaderOutputDirectory}]"

echo "
###########################
#   Generating files...   #
###########################
"
${glslcExe} -o "${shaderOutputDirectory}" "${shaderDirectory}"