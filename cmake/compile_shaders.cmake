
# UPDATE ME
set(BUILD_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin/debug)

function(compile_shader SHADER)
    set(SHADER_RESOURCE_PATH ${CMAKE_SOURCE_DIR}/assets/shaders/${SHADER})
    set(SHADER_OUTPUT_PATH ${BUILD_PATH}/shaders/${SHADER}.spv)
    find_program(GLSLC glslc)
    get_filename_component(SHADER_OUTPUT_DIRECTORY ${SHADER_OUTPUT_PATH} DIRECTORY)
    file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIRECTORY})
    exec_program(${GLSLC} ARGS -o ${SHADER_OUTPUT_PATH} ${SHADER_RESOURCE_PATH})
endfunction()

compile_shader(simple_shader.vert)
compile_shader(simple_shader.frag)