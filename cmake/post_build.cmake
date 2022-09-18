# Run after all other rules within the target have been executed (PRE_BUILD & PRE_LINK).

set(CMAKE_SCRIPTS_DIR ${CMAKE_CURRENT_LIST_DIR})
string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE_DIR_NAME)

set(SHADER_ASSETS_DIR ${ASSETS_DIR}/shaders)
set(SHADER_OUTPUT_DIR ${BIN_DIR}/${BUILD_TYPE_DIR_NAME}/shaders)

include(${CMAKE_SCRIPTS_DIR}/compile_shaders.cmake)

file(COPY ${ASSETS_DIR}/textures DESTINATION ${BIN_DIR}/${BUILD_TYPE_DIR_NAME})

file(COPY ${ASSETS_DIR}/models DESTINATION ${BIN_DIR}/${BUILD_TYPE_DIR_NAME})