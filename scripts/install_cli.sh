#!/bin/bash

# Exit when any command fails
set -e

echo "
#######################################
#  Setting up...                      #
#######################################
"
if [[ "$(pwd)" == */scripts ]]; then
  cd ..
fi

cd cli

workingDirectory="$(pwd)"
echo "-- Working from directory [${workingDirectory}]"

sourceDirectory="${workingDirectory}"
echo "-- Using CMake source directory [${sourceDirectory}]"

buildDirectory="${workingDirectory}/build"
echo "-- Using CMake build directory [${buildDirectory}]"

buildType="Release"
echo "-- Using build type [${buildType}]"

echo "
#######################################
#  Generating build files...          #
#######################################
"
cmake -B "${buildDirectory}" -S "${sourceDirectory}" -DCMAKE_BUILD_TYPE="${buildType}"

echo "
#######################################
#  Building binaries...               #
#######################################
"
cmake --build "${buildDirectory}" --config "${buildType}"

echo "
#######################################
#  Installing binaries...             #
#######################################
"
cmake --install "${buildDirectory}" --config "${buildType}"

echo ""