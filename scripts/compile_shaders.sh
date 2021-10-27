#!/bin/bash

# Exit when any command fails
set -e

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

cmakeScript="${workingDirectory}/cmake/compile_shaders.cmake"
echo "-- Using CMake script [${cmakeScript}]"

echo "
############################
#   Compiling shaders...   #
############################
"
cmake -P "${cmakeScript}"