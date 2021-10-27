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

readonly CLION_ENVIRONMENT="CLion"
readonly VISUAL_STUDIO_CODE_ENVIRONMENT="Visual Studio Code"
readonly VISUAL_STUDIO_ENVIRONMENT="Visual Studio"
readonly XCODE_ENVIRONMENT="XCode"

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

PS3="Select environment: "
select environment in "${CLION_ENVIRONMENT}" "${VISUAL_STUDIO_CODE_ENVIRONMENT}" "${VISUAL_STUDIO_ENVIRONMENT}" "${XCODE_ENVIRONMENT}"
do
  if [[ "${environment}" == "${CLION_ENVIRONMENT}" ]]; then
    environment="clion"
  elif [[ "${environment}" == "${VISUAL_STUDIO_CODE_ENVIRONMENT}" ]]; then
    environment="vscode"
  elif [[ "${environment}" == "${VISUAL_STUDIO_ENVIRONMENT}" ]]; then
    environment="vstudio"
  elif [[ "${environment}" == "${XCODE_ENVIRONMENT}" ]]; then
    environment="xcode"
  else
    echo "Invalid environment"
    exit 1
  fi
  break
done

if [[ -z ${buildType} ]]; then
  buildType="${DEBUG_BUILD_TYPE}"
fi
if [[ ${buildType} == "debug" || ${buildType} == "DEBUG" ]]; then
  buildType="${DEBUG_BUILD_TYPE}"
fi
if [[ ${buildType} == "release" || ${buildType} == "RELEASE" ]]; then
  buildType="${RELEASE_BUILD_TYPE}"
fi

if [[ "${environment}" == "${WINDOWS_PLATFORM}" && -z "${architecture}" ]]; then
  architecture="${WINDOWS_DEFAULT_ARCHITECTURE}"
fi

if [[ "$(pwd)" == */setup ]]; then
  cd ..
fi
if [[ "$(pwd)" != */scripts ]]; then
  cd scripts
fi

./generate_project.sh -b "${buildType}" -e "${environment}" "$([ -n "${generator}" ] && echo "-g ${generator}")" "$([ -n "${architecture}" ] && echo "-a ${architecture}")"

./build_exe -b "${buildType}" -e "${environment}"