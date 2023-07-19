# Copyright (c) 2023 adasdead
# This software is licensed under the MIT License.

cmake_minimum_required(VERSION 3.10)
project(mines VERSION 1.0 LANGUAGES C)

set(PROJECT_3RD_PARTY_DIR ${PROJECT_SOURCE_DIR}/3rd-party)
set(PROJECT_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(PROJECT_ASSETS_DIR ${PROJECT_SOURCE_DIR}/assets)
set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BIN_DIR})

if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
endif()

add_subdirectory(${PROJECT_3RD_PARTY_DIR})
add_subdirectory(${PROJECT_SOURCE_DIR})