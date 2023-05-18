# Copyright (c) 2023 adasdead
# This software is licensed under the MIT License.

#############################################################################
# GLFW                                                                      #
#############################################################################

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(${PROJECT_DEPS_DIR}/glfw)

#############################################################################
# GLAD                                                                      #
#############################################################################

add_library(glad STATIC glad/glad.c)

#############################################################################
# Discord SDK                                                               #
#############################################################################

if(EXISTS "${PROJECT_DEPS_DIR}/discord_sdk" AND WIN32)
    message(STATUS "Discord SDK found!")

    set(DISCORD_SDK_FOUND ON PARENT_SCOPE)
    set(DISCORD_SDK_DIR "${PROJECT_DEPS_DIR}/discord_sdk")
    set(DISCORD_SDK_INCLUDE_DIR "${DISCORD_SDK_DIR}/c" CACHE INTERNAL "")

    set(DISCORD_SDK_LIBRARY
        "${DISCORD_SDK_DIR}/lib/x86_64/discord_game_sdk.dll.lib"
        CACHE INTERNAL ""
    )
    set(DISCORD_SDK_DLL
        "${DISCORD_SDK_DIR}/lib/x86_64/discord_game_sdk.dll"
        CACHE INTERNAL ""
    )
endif()

#############################################################################
# Other                                                                     #
#############################################################################

include_directories(".")