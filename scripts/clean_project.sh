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

echo ""
echo "!! MAKE SURE THE WORKING DIRECTORY IS PROJECT ROOT"
echo "!! MAKE SURE THE WORKING DIRECTORY IS PROJECT ROOT"
echo "!! MAKE SURE THE WORKING DIRECTORY IS PROJECT ROOT"
echo ""
read -p "Press Enter to continue"

deleteFile() {
  local file="${workingDirectory}/${1}"
  if [ -f "${file}" ]; then
    rm -f ${file}
    echo "-- Deleted file [${file}]"
  fi
}

deleteDirectory() {
  local directory="${workingDirectory}/${1}"
  if [ -e ${directory} ]; then
    rm -rf ${directory}
    echo "-- Deleted directory [${directory}]"
  fi
}

echo "
#################################
#   Cleaning project files...   #
#################################
"
deleteDirectory "build"
deleteDirectory "Debug"
deleteDirectory "Release"

# CMake
deleteDirectory "CMakeFiles"
deleteDirectory "CMakeScripts"
deleteFile "CMakeCache.txt"
deleteFile "CMakeDoxyfile.in"
deleteFile "CMakeDoxygenDefaults.cmake"
deleteFile "cmake_install.cmake"
deleteFile "cmake_uninstall.cmake"

# Makefile generator
deleteFile "Makefile"

# CLion
deleteDirectory "cmake-build-debug"
deleteDirectory "cmake-build-release"

# Visual Studio
deleteFile "*.sln"
deleteFile "*.vcxproj*"
deleteFile "CMakeSettings.json"
deleteDirectory "*.dir"
deleteDirectory "Win32"
deleteDirectory "x64"

# XCode
deleteDirectory "*.build"
deleteDirectory "*.xcodeproj"