# Copyright (c) 2023 adasdead
# This software is licensed under the MIT License. (see the LICENSE file)

find_package(OpenGL REQUIRED)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_executable(${PROJECT_NAME} main.c util/logger.c)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG=1)
else()
    add_executable(${PROJECT_NAME} WIN32 main.c)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG=0)
endif()

string(CONCAT PROJECT_VERSION_STRING
    ${PROJECT_VERSION_MAJOR} "." ${PROJECT_VERSION_MINOR}
)

target_sources(${PROJECT_NAME} PRIVATE
    graphics/shader.c
    graphics/texture.c
    graphics/renderer.c

    util/resources.c
    util/matrix4x4.c

    game/game.c
    game/window.c
    game/field.c
    game/smile.c
    game/counter.c
    game/border.c
)

target_link_libraries(${PROJECT_NAME}
    ${CMAKE_DL_LIBS}
    ${MATH_LIB}
    OpenGL::GL
    glfw
    glad
)

configure_file(version.h.in version.h @ONLY)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR} ${PROJECT_DEPS_DIR} ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_command(TARGET ${PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_ASSETS_DIR} $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)