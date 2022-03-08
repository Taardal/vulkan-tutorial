# Run after sources have been compiled but before linking the binary or running the librarian or archiver tool of a static library.
# This is not defined for targets created by the add_custom_target() command.

message("PRE LINK")

set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)
set(SHADER_ASSETS_DIR ${ASSETS_DIR}/shaders)

function(print_filenames DIRECTORY WILDCARD)
    file(GLOB files "${DIRECTORY}/${WILDCARD}")
    foreach (file ${files})
        get_filename_component(FILENAME "${file}" NAME)
        message("${FILENAME}")
    endforeach ()
endfunction()

print_filenames(${SHADER_ASSETS_DIR} *.frag)
print_filenames(${SHADER_ASSETS_DIR} *.vert)