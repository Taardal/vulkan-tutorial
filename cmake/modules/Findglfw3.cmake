message("Finding GLFW library")

set(PKG_NAME ${CMAKE_FIND_PACKAGE_NAME})
file(TO_CMAKE_PATH $ENV{ProgramFiles\(x86\)}\\GLFW\\include HEADER_DIR)
file(TO_CMAKE_PATH $ENV{ProgramFiles\(x86\)}\\GLFW\\lib BINARY_DIR)

message("Finding include directory")
find_path(
    ${PKG_NAME}_INCLUDE_DIR
    NAMES
    GLFW/${PKG_NAME}.h
    PATHS
    "C:/Program Files (x86)/GLFW/include"
    ${HEADER_DIR}
    
)
message("Found include directory: ${${PKG_NAME}_INCLUDE_DIR}")
message("---")


message("Finding library")
find_library(
    ${PKG_NAME}_LIBRARY
    NAMES
    ${PKG_NAME}
    PATHS
    "C:/Program Files (x86)/GLFW/lib"
    ${BINARY_DIR}
)
message("Found library: ${${PKG_NAME}_LIBRARY}")
message("---")


message("Finding package")
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    ${PKG_NAME} 
    DEFAULT_MSG 
    ${${PKG_NAME}_LIBRARY} 
    ${glfw3_INCLUDE_DIR}
)
message("Found package: ${${PKG_NAME}_FOUND}")

