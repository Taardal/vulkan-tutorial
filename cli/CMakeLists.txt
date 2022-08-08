cmake_minimum_required(VERSION 3.20.2)
set(CMAKE_CXX_STANDARD 17)

project(vulkandemo-cli)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(EXE_NAME vd)

add_executable(
        ${PROJECT_NAME}
        ${SRC_DIR}/main.cpp
        ${SRC_DIR}/AddLibraryCommand.cpp
        ${SRC_DIR}/AddLibraryCommand.h
        ${SRC_DIR}/BuildProjectCommand.cpp
        ${SRC_DIR}/BuildProjectCommand.h
        ${SRC_DIR}/CompileShadersCommand.cpp
        ${SRC_DIR}/CompileShadersCommand.h
        ${SRC_DIR}/Environment.h
        ${SRC_DIR}/FileSystem.cpp
        ${SRC_DIR}/FileSystem.h
        ${SRC_DIR}/InstallDependenciesCommand.cpp
        ${SRC_DIR}/InstallDependenciesCommand.h
        ${SRC_DIR}/InstallGLFWCommand.cpp
        ${SRC_DIR}/InstallGLFWCommand.h
        ${SRC_DIR}/RunProjectCommand.cpp
        ${SRC_DIR}/RunProjectCommand.h
)

set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_NAME ${EXE_NAME}
        RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR}/debug
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR}/release
)

target_compile_definitions(
        ${PROJECT_NAME}
        PRIVATE
        VDC_EXE_NAME="${EXE_NAME}"
)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
install(
        TARGETS ${PROJECT_NAME}
        DESTINATION ..
)

set(CLI_DIR ${LIB_DIR}/cli)
add_subdirectory(${CLI_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CLI_DIR}/include)
target_link_libraries(${PROJECT_NAME} cli)