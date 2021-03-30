# This file is part of the Blocks game.
# Copyright (C) Sergei Blagodarin.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19)

project(Blocks LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(YTTRIUM_AUDIO_AULOS ON)
set(YTTRIUM_IMAGE_ICO ON)
set(YTTRIUM_IMAGE_JPEG ON)
set(YTTRIUM_IMAGE_TGA ON) # For generated images.

include(FetchContent)
FetchContent_Declare(Yttrium GIT_REPOSITORY https://github.com/blagodarin/yttrium.git GIT_TAG e263502da7be5404c32d61d15a1ea8146695fb3e)
FetchContent_MakeAvailable(Yttrium)

source_group("" FILES blocks.rc)
source_group("src" REGULAR_EXPRESSION "src/")
source_group("src/screens" REGULAR_EXPRESSION "src/screens/")
add_executable(blocks WIN32
	src/game.cpp
	src/game.hpp
	src/graphics.cpp
	src/graphics.hpp
	src/logic.cpp
	src/logic.hpp
	src/main.cpp
	src/screen.hpp
	src/screens/game.cpp
	src/screens/game.hpp
	src/screens/game_over.cpp
	src/screens/game_over.hpp
	src/screens/help.cpp
	src/screens/help.hpp
	src/screens/logo.cpp
	src/screens/logo.hpp
	src/screens/main_menu.cpp
	src/screens/main_menu.hpp
	src/screens/pause.cpp
	src/screens/pause.hpp
	src/screens/play_menu.cpp
	src/screens/play_menu.hpp
	src/screens/top_scores.cpp
	src/screens/top_scores.hpp
	$<$<BOOL:${WIN32}>:blocks.rc>
	)
target_link_libraries(blocks PRIVATE Y_application Y_audio Y_gui Y_image Y_main Y_renderer Y_storage)
set_target_properties(blocks PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

yttrium_target_package(blocks
	GROUP "embed"
	OUTPUT blocks.ypq
	INDEX data/index.txt
	EMBED)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT blocks)
