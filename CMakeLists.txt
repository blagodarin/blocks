# This file is part of the Blocks game.
# Copyright (C) Sergei Blagodarin.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.21)

project(Blocks VERSION 0.0.0.0 LANGUAGES CXX)

set(YTTRIUM_AUDIO_SEIRSYNTH ON)
set(YTTRIUM_IMAGE_ICO ON)
set(YTTRIUM_IMAGE_PNG ON) # For screenshots (not JPEG for smaller binary size).

include(FetchContent)
FetchContent_Declare(Yttrium GIT_REPOSITORY https://github.com/blagodarin/yttrium.git GIT_TAG 36a1ebedccf820d8199c2e7f558841fd21fd73d3)
FetchContent_MakeAvailable(Yttrium)

seir_defaults()

source_group("src" REGULAR_EXPRESSION "src/")
source_group("src/screens" REGULAR_EXPRESSION "src/screens/")
add_executable(Blocks WIN32
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
	src/textures.cpp
	src/textures.hpp
	)
target_link_libraries(Blocks PRIVATE Seir::audio Seir::base Seir::data Seir::image Seir::math Seir::u8main Y_application Y_gui Y_renderer)
seir_target(Blocks STATIC_RUNTIME ON)

if(WIN32)
	configure_file(blocks.rc.in blocks.rc)
	target_include_directories(Blocks PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
	target_sources(Blocks PRIVATE blocks.rc.in ${CMAKE_CURRENT_BINARY_DIR}/blocks.rc)
	source_group("" FILES blocks.rc.in ${CMAKE_CURRENT_BINARY_DIR}/blocks.rc)
endif()

yttrium_target_package(Blocks
	GROUP "embed"
	OUTPUT blocks.yp
	INDEX data/index.st
	EMBED)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT Blocks)
