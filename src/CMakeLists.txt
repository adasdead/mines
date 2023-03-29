find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(${PROJECT_NAME} WIN32 main.c)

target_sources(${PROJECT_NAME} PRIVATE
    glad/glad.c

    graphics/shader.c
    graphics/texture.c

    logger.c

    field.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
    .
)

set_target_properties(${PROJECT_NAME} PROPERTIES
                          COMPILE_OPTIONS "-fanalyzer"
                          EXCLUDE_FROM_DEFAULT_BUILD true)


target_link_libraries(${PROJECT_NAME}
    ${CMAKE_DL_LIBS}
    ${MATH_LIB}
    OpenGL::GL
    glfw
    m
)