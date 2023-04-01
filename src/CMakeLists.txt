find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(${PROJECT_NAME} WIN32 main.c)

target_sources(${PROJECT_NAME} PRIVATE
    glad/glad.c

    graphics/shader.c
    graphics/texture.c

    util/resources.c
    util/logger.c

    game/game.c
    game/window.c
    game/field.c
    game/smile.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    COMPILE_OPTIONS "-fanalyzer"
    EXCLUDE_FROM_DEFAULT_BUILD true
)


target_link_libraries(${PROJECT_NAME}
    ${CMAKE_DL_LIBS}
    ${MATH_LIB}
    OpenGL::GL
    glfw
    m
)

add_custom_command(TARGET ${PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_ASSETS_DIR} $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)