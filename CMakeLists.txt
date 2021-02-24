# This file is part of the Blocks game.
# Copyright (C) Sergei Blagodarin.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19)

project(Blocks LANGUAGES CXX)

include(FetchContent)

set(CMAKE_FOLDER "Yttrium")
FetchContent_Declare(Yttrium GIT_REPOSITORY https://github.com/blagodarin/yttrium.git GIT_TAG 1f1c79b2619168af923632ce8f9d8dee507e284b)
FetchContent_MakeAvailable(Yttrium)
unset(CMAKE_FOLDER)

primal_set_defaults()

add_executable(blocks WIN32
	src/main.cpp
	)
target_link_libraries(blocks PRIVATE ymain)
set_target_properties(blocks PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT blocks)
