#!/bin/bash

# Exit when any command fails
set -e

readonly DEBUG_BUILD_TYPE="Debug"
readonly RELEASE_BUILD_TYPE="Release"

readonly CLION_ENVIRONMENT="clion"
readonly VISUAL_STUDIO_CODE_ENVIRONMENT="vscode"
readonly VISUAL_STUDIO_ENVIRONMENT="vstudio"
readonly XCODE_ENVIRONMENT="xcode"

while getopts ":b:e:" flag; do
  case "${flag}" in
    b) 
      buildType="$OPTARG"
      ;;
    e)
      environment="$OPTARG"
      ;;
    \?) echo "Invalid option -$OPTARG" >&2
    exit 1;;
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

resolveBuildDirectory
echo "-- Using CMake build directory [${buildDirectory}]"

echo "
###########################
#   Building project...   #
###########################
"
cmake --build "${buildDirectory}" --config "${buildType}"