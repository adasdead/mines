# Copyright (c) 2023 adasdead
# This software is licensed under the MIT License.

find_package(OpenGL REQUIRED)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_executable(${PROJECT_NAME} main.c util/logger.c)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG=1)
else()
    add_executable(${PROJECT_NAME} WIN32 main.c mines.rc)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG=0)
endif()

if (DISCORD_SDK_FOUND)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DISCORD=1)
    target_sources(${PROJECT_NAME} PRIVATE util/discord.c)
    target_link_libraries(${PROJECT_NAME} ${DISCORD_SDK_LIBRARY})
    add_custom_command(TARGET ${PROJECT_NAME}
        COMMAND ${CMAKE_COMMAND} -E copy
        ${DISCORD_SDK_DLL}
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

configure_file(version.h.in version.h @ONLY)

target_sources(${PROJECT_NAME} PRIVATE
    game/game.c
    game/window.c
    game/field.c
    game/smile.c
    game/counter.c
    game/border.c

    graphics/shader.c
    graphics/texture.c
    graphics/renderer.c

    util/resources.c
    util/matrix4x4.c
)

target_link_libraries(${PROJECT_NAME}
    ${CMAKE_DL_LIBS}
    OpenGL::GL
    glfw
    glad
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_DEPS_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${DISCORD_SDK_INCLUDE_DIR}
)

add_custom_command(TARGET ${PROJECT_NAME}
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_ASSETS_DIR}
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)