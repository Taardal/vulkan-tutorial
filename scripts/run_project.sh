#!/bin/bash

# Exit when any command fails
set -e

readonly DEBUG_BUILD_TYPE="Debug"
readonly RELEASE_BUILD_TYPE="Release"

while getopts ":b:e:" flag; do
  case "${flag}" in
    b) 
      buildType="$OPTARG"
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

resolveBuildType
echo "-- Using build type [${buildType}]"

executableDirectory="${workingDirectory}/bin/$(echo ${buildType} | awk '{print tolower($0)}')"
echo "-- Using executable directory [${executableDirectory}]"

echo "
##########################
#   Running project...   #
##########################
"
cd "${executableDirectory}"
./vulkandemo