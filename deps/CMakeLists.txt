# GLFW ------------------------------------------------------------------------

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(${PROJECT_DEPS_DIR}/glfw)

# GLAD ------------------------------------------------------------------------

add_library(glad STATIC glad/glad.c)

# INCLUDE ---------------------------------------------------------------------

include_directories(".")