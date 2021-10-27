#!/bin/bash

# Exit when any command fails
set -e

readonly DEBUG_BUILD_TYPE="Debug"
readonly RELEASE_BUILD_TYPE="Release"

readonly WINDOWS_PLATFORM="Windows"
readonly MAC_PLATFORM="macOS"
readonly LINUX_PLATFORM="Linux"

readonly WINDOWS_DEFAULT_GENERATOR="Visual Studio 16 2019"
readonly WINDOWS_DEFAULT_ARCHITECTURE="x64"

while getopts ":a:b:g:" flag; do
  case "${flag}" in
    b) 
      buildType="$OPTARG"
      ;;
    b) 
      generator="$OPTARG"
      ;;
    a) 
      architecture="$OPTARG"
      ;;
    \?) 
      echo "Invalid option -$OPTARG" >&2
      exit 1
      ;;
  esac
done

PS3="Select platform: "
select platform in "${WINDOWS_PLATFORM}" "${MAC_PLATFORM}" "${LINUX_PLATFORM}"
do
  if [[ "${platform}" == "${WINDOWS_PLATFORM}" && -z "${architecture}" ]]; then
    architecture="${WINDOWS_DEFAULT_ARCHITECTURE}"
  fi
  break
done

resolveBuildType() {
  if [[ -z ${buildType} ]]; then
    buildType="${DEBUG_BUILD_TYPE}"
  fi
  if [[ ${buildType} == "debug" || ${buildType} == "DEBUG" ]]; then
    buildType="${DEBUG_BUILD_TYPE}"
  fi
  if [[ ${buildType} != "${DEBUG_BUILD_TYPE}" && ${buildType} != "${RELEASE_BUILD_TYPE}" ]]; then
    echo "Invalid build type [${buildType}], expected [${DEBUG_BUILD_TYPE},${RELEASE_BUILD_TYPE}]"
    exit 1
  fi
}

echo "
#####################
#   Setting up...   #
#####################
"
if [[ "$(pwd)" == */setup ]]; then
  cd ..
fi
if [[ "$(pwd)" == */scripts ]]; then
  cd ..
fi
cd lib/glfw
workingDirectory="$(pwd)"
echo "-- Working from directory [${workingDirectory}]"

sourceDirectory="${workingDirectory}"
echo "-- Using CMake source directory [${sourceDirectory}]"

buildDirectory="${workingDirectory}/build"
echo "-- Using CMake build directory [${buildDirectory}]"

resolveBuildType
echo "-- Using build type [${buildType}]"

if [[ -n "${generator}" ]]; then
  echo "-- Using generator [${generator}]"
fi

if [[ -n "${architecture}" ]]; then
  echo "-- Using architecture [${architecture}]"
fi

echo "
#################################
#   Generating build files...   #
#################################
"
cmake -DGLFW_BUILD_DOCS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE="${buildType}" -S "${sourceDirectory}" -B "${buildDirectory}" "$([ -n "${generator}" ] && echo "-G ${generator}")" "$([ -n "${architecture}" ] && echo "-A ${architecture}")"

echo "
############################
#   Building binaries...   #
############################
"
cmake --build "${buildDirectory}" --config "${buildType}"

echo "
##############################
#   Installing binaries...   #
##############################
"
cmake --install "${buildDirectory}" --config "${buildType}"
