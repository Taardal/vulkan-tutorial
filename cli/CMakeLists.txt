cmake_minimum_required(VERSION 3.20.2)
set(CMAKE_CXX_STANDARD 17)

project(vulkandemo-cli)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(EXE_NAME vd)

add_executable(
        ${PROJECT_NAME}
        ${SRC_DIR}/main.cpp
        ${SRC_DIR}/Environment.h
)

set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_NAME ${EXE_NAME}
        RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR}/debug
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR}/release
)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
install(
        TARGETS ${PROJECT_NAME}
        DESTINATION ..
)