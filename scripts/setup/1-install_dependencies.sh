#!/bin/bash

# Exit when any command fails
set -e

readonly GLFW_TAG="v3.3.4"
readonly GLM_TAG="v0.9.9.8"
readonly SPDLOG_TAG="v3.3.4"

if [[ "$(pwd)" == */setup ]]; then
  cd ..
fi
if [[ "$(pwd)" == */scripts ]]; then
  cd ..
fi

echo "
####################################
#   Installing git submodules...   #
####################################
"
git submodule update --init

echo "
#########################################
#   Setting git submodule versions...   #
#########################################
"
cd lib

cd glfw
git checkout "${GLFW_TAG}"
echo "-- Using GLFW version [${GLFW_TAG}]"

cd ..

cd glm
git checkout "${GLM_TAG}"
echo "-- Using GLM version [${GLM_TAG}]"

cd ..

cd spdlog
git checkout "${SPDLOG_TAG}"
echo "-- Using spdlog version [${SPDLOG_TAG}]"