find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} WIN32 main.c)

target_sources(${PROJECT_NAME} PRIVATE
    graphics/shader.c
    graphics/texture.c

    util/resources.c
    util/logger.c
    util/matrix4x4.c

    game/game.c
    game/window.c
    game/field.c
    game/smile.c
    game/counter.c

    glfw_callbacks.c
)

target_sources(${PROJECT_NAME} PRIVATE
    ${PROJECT_DEPS_DIR}/glad/glad.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR} ${PROJECT_DEPS_DIR}
)

# set_target_properties(${PROJECT_NAME} PROPERTIES
#     COMPILE_OPTIONS "-fanalyzer"
#     EXCLUDE_FROM_DEFAULT_BUILD true
# )

target_link_libraries(${PROJECT_NAME}
    ${CMAKE_DL_LIBS}
    ${MATH_LIB}
    OpenGL::GL
    glfw
)

add_custom_command(TARGET ${PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_ASSETS_DIR} $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)