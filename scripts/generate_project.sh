#!/bin/bash

# Exit when any command fails
set -e

readonly DEBUG_BUILD_TYPE="Debug"
readonly RELEASE_BUILD_TYPE="Release"

readonly CLION_ENVIRONMENT="clion"
readonly VISUAL_STUDIO_CODE_ENVIRONMENT="vscode"
readonly VISUAL_STUDIO_ENVIRONMENT="vstudio"
readonly XCODE_ENVIRONMENT="xcode"

readonly VISUAL_STUDIO_DEFAULT_GENERATOR="Visual Studio 16 2019"
readonly VISUAL_STUDIO_DEFAULT_ARCHITECTURE="x64"

while getopts ":a:b:e:g:" flag; do
  case "${flag}" in
    a) 
      architecture="$OPTARG"
      ;;
    b) 
      buildType="$OPTARG"
      ;;
    e)
      environment="$OPTARG"
      ;;
    g) 
      generator="$OPTARG"
      ;;
    \?) 
      echo "Invalid option -$OPTARG" >&2
      exit 1
      ;;
  esac
done

resolveBuildType() {
  if [[ -z ${buildType} ]]; then
    buildType="${DEBUG_BUILD_TYPE}"
  fi
  if [[ ${buildType} == "debug" || ${buildType} == "DEBUG" ]]; then
    buildType="${DEBUG_BUILD_TYPE}"
  fi
  if [[ ${buildType} == "release" || ${buildType} == "RELEASE" ]]; then
    buildType="${RELEASE_BUILD_TYPE}"
  fi
}

resolveEnvironment() {
  if [[ -z "${environment}" ]]; then
    environment="${VISUAL_STUDIO_CODE_ENVIRONMENT}"
  fi
}

resolveBuildDirectory() {
  if [[ "${environment}" == "${CLION_ENVIRONMENT}" ]]; then
    buildDirectory="${workingDirectory}/cmake-build-$(echo ${buildType} | awk '{print tolower($0)}')"  
  elif [[ "${environment}" == "${VISUAL_STUDIO_ENVIRONMENT}" ]]; then
    buildDirectory="${workingDirectory}"
  elif [[ "${environment}" == "${XCODE_ENVIRONMENT}" ]]; then
    buildDirectory="${workingDirectory}"
  else 
    buildDirectory="${workingDirectory}/build"
  fi
}

resolveGenerator() {
  if [[ "${environment}" == "${VISUAL_STUDIO_ENVIRONMENT}" && -z "${generator}" ]]; then
    generator="${VISUAL_STUDIO_DEFAULT_GENERATOR}"
  fi
}

resolveArchitecture() {
  if [[ "${environment}" == "${VISUAL_STUDIO_ENVIRONMENT}" && -z "${architecture}" ]]; then
    architecture="${VISUAL_STUDIO_DEFAULT_ARCHITECTURE}"
  fi
}

echo "
#####################
#   Setting up...   #
#####################
"
if [[ "$(pwd)" == */scripts ]]; then
  cd ..
fi
workingDirectory="$(pwd)"
echo "-- Working from directory [${workingDirectory}]"

resolveEnvironment
echo "-- Generating files for [${environment}]"

resolveBuildType
echo "-- Using build type [${buildType}]"

sourceDirectory="${workingDirectory}"
echo "-- Using CMake source directory [${sourceDirectory}]"

resolveBuildDirectory
echo "-- Using CMake build directory [${buildDirectory}]"

resolveGenerator
if [[ -n "${generator}" ]]; then
  echo "-- Using generator [${generator}]"
fi

resolveArchitecture
if [[ -n "${architecture}" ]]; then
  echo "-- Using architecture [${architecture}]"
fi

echo "
#################################
#   Generating build files...   #
#################################
"
cmake -DCMAKE_BUILD_TYPE="${buildType}" -S "${sourceDirectory}" -B "${buildDirectory}" "$([ -n "${generator}" ] && echo "-G ${generator}")" "$([ -n "${architecture}" ] && echo "-A ${architecture}")"