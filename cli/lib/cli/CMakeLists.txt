cmake_minimum_required(VERSION 3.20.2)
set(CMAKE_CXX_STANDARD 17)

project(cli)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(BIN_DIR ${PROJECT_SOURCE_DIR}/bin)

add_library(
        ${PROJECT_NAME}
        ${SRC_DIR}/App.cpp
        ${SRC_DIR}/App.h
        ${SRC_DIR}/Command.cpp
        ${SRC_DIR}/Command.h
        ${SRC_DIR}/Context.cpp
        ${SRC_DIR}/Context.h
        ${SRC_DIR}/Help.cpp
        ${SRC_DIR}/Help.h
        ${SRC_DIR}/Option.cpp
        ${SRC_DIR}/Option.h
)

set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_NAME ${PROJECT_NAME}
        RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR}/debug
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR}/release
)

target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR})