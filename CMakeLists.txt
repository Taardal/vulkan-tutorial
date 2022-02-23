cmake_minimum_required(VERSION 3.20.2)
set(CMAKE_CXX_STANDARD 17)

project(vulkandemo)

option(
        BUILD_GLFW_SRC 
        "Build GLFW as part of this project instead of using installed binaries" 
        OFF
)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_DIR ${PROJECT_SOURCE_DIR}/cmake)
set(ASSETS_DIR ${PROJECT_SOURCE_DIR}/assets)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR}/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR}/release)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR}/lib/debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR}/lib/release)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR}/lib/debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR}/lib/release)

# Set cmake modules
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_DIR}/modules)

add_executable(
        ${PROJECT_NAME}
        ${SRC_DIR}/main.cpp
        ${SRC_DIR}/App.cpp
        ${SRC_DIR}/App.h
        ${SRC_DIR}/Assert.h
        ${SRC_DIR}/Environment.h
        ${SRC_DIR}/FileSystem.cpp
        ${SRC_DIR}/FileSystem.h
        ${SRC_DIR}/Log.cpp
        ${SRC_DIR}/Log.h
        ${SRC_DIR}/VulkanContext.cpp
        ${SRC_DIR}/VulkanContext.h
        ${SRC_DIR}/Window.cpp
        ${SRC_DIR}/Window.h
)

add_custom_command(
        TARGET ${PROJECT_NAME}
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_DIR}/pre_build.cmake
)

add_custom_command(
        TARGET ${PROJECT_NAME}
        PRE_LINK
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_DIR}/pre_link.cmake
)

add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_DIR}/post_build.cmake
)

install(
        TARGETS ${PROJECT_NAME}
        DESTINATION ${BIN_DIR}
)

if (${BUILD_GLFW_SRC})
        set(GLFW_DIR ${LIB_DIR}/glfw)
        set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        add_subdirectory(${GLFW_DIR})
        target_compile_definitions(${PROJECT_NAME} PUBLIC GLFW_INCLUDE_NONE)
        target_include_directories(${PROJECT_NAME} PUBLIC ${GLFW_DIR}/include)
        target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})
else()
        set(GLFW_DIR $ENV{GLFW_HOME})
        set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${GLFW_DIR})
        find_package(glfw3 REQUIRED)
        target_link_libraries(${PROJECT_NAME} glfw)
endif ()

set(GLM_DIR ${LIB_DIR}/glm)
target_include_directories(${PROJECT_NAME} PUBLIC ${GLM_DIR})

set(SPDLOG_DIR ${LIB_DIR}/spdlog)
target_include_directories(${PROJECT_NAME} PUBLIC ${SPDLOG_DIR}/include)

find_package(Vulkan REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)