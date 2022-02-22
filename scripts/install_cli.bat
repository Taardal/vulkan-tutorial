@echo off

Rem The folder your scripts operates on
set foo=%CD%
echo %foo%

Rem The folder your script is located in
set bar=%~dp0
echo %bar%

Rem ... ??
set asd=%~dp0
set asd=%asd:~0,-1%
echo %asd%

echo ""
echo #######################################
echo #  Setting up...                      #
echo #######################################
echo ""
cd %bar%\..\cli

set workingDirectory=%CD%
echo "-- Working from directory [%workingDirectory%]"

set sourceDirectory=%workingDirectory%
echo "-- Using CMake source directory [%sourceDirectory%]"

set buildDirectory=%workingDirectory%/build
echo "-- Using CMake build directory [%buildDirectory%]"

set buildType=Release
echo "-- Using build type [%buildType%]"

echo ""
echo #######################################
echo #  Generating build files...          #
echo #######################################
echo ""
cmake -B %buildDirectory% -S %sourceDirectory% -DCMAKE_BUILD_TYPE=%buildType%

echo ""
echo #######################################
echo #  Building binaries...               #
echo #######################################
echo ""
cmake --build %buildDirectory% --config %buildType%

echo ""
echo #######################################
echo #  Installing binaries...             #
echo #######################################
echo ""
cmake --install %buildDirectory% --config %buildType%

echo ""