# This file is part of the Blocks game.
# Copyright (C) Sergei Blagodarin.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19)

project(Blocks LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(YTTRIUM_WITH_JPEG ON)
set(YTTRIUM_WITH_OGGVORBIS OFF)

include(FetchContent)
FetchContent_Declare(Yttrium GIT_REPOSITORY https://github.com/blagodarin/yttrium.git GIT_TAG a0209ba2cfd20a6820848ccb2da2ff01c3463cec)
FetchContent_MakeAvailable(Yttrium)

add_executable(blocks WIN32
	src/main.cpp
	)
target_link_libraries(blocks PRIVATE ymain)
set_target_properties(blocks PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT blocks)
