
# UPDATE ME
set(BUILD_PATH ${CMAKE_CURRENT_BINARY_DIR}/cmake-build-debug/Debug)

set(SHADER_RESOURCE_PATH ${CMAKE_SOURCE_DIR}/res/shaders/${SHADER})
set(SHADER_OUTPUT_PATH ${BUILD_PATH}/shaders/${SHADER}.spv)

function(compile_shader SHADER)
    find_program(GLSLC glslc)
    get_filename_component(SHADER_OUTPUT_DIRECTORY ${SHADER_OUTPUT_PATH} DIRECTORY)
    file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIRECTORY})
    exec_program(${GLSLC} ARGS -o ${SHADER_OUTPUT_PATH} ${SHADER_RESOURCE_PATH})
endfunction()

compile_shader(simple_shader.vert)
compile_shader(simple_shader.frag)