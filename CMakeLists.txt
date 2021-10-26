cmake_minimum_required(VERSION 3.20.2)
set(CMAKE_CXX_STANDARD 17)

project(vulkandemo)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(RES_DIR ${PROJECT_SOURCE_DIR}/res)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(BIN_DIR ${PROJECT_SOURCE_DIR}/bin)

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

install(TARGETS ${PROJECT_NAME} DESTINATION ${BIN_DIR})

set(GLFW_DIR ${LIB_DIR}/glfw)
find_package(glfw3 REQUIRED)
target_link_libraries(${PROJECT_NAME} glfw)

set(GLM_DIR ${LIB_DIR}/glm)
target_include_directories(${PROJECT_NAME} PUBLIC ${GLM_DIR})

set(SPD_LOG_DIR ${LIB_DIR}/spdlog)
target_include_directories(${PROJECT_NAME} PUBLIC ${SPD_LOG_DIR}/include)

find_package(Vulkan REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)