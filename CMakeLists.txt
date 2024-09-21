# This file is part of the Blocks game.
# Copyright (C) Sergei Blagodarin.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.25)

project(Blocks VERSION 0.0.0.0 LANGUAGES CXX)

set(SEIR_IMAGE_ICO ON)
set(SEIR_IMAGE_PNG ON) # For screenshots (not JPEG for smaller binary size).
set(SEIR_STATIC_RUNTIME ON)

include(FetchContent)
FetchContent_Declare(Seir GIT_REPOSITORY https://github.com/blagodarin/seir.git GIT_TAG f8c3340a6dc3f87054c636a2b7b02c9810d73ecd)
FetchContent_MakeAvailable(Seir)

seir_defaults()

set(SOURCES
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
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})
add_executable(Blocks WIN32 ${SOURCES})
target_link_libraries(Blocks PRIVATE Seir::audio Seir::gui Seir::image Seir::io Seir::package Seir::renderer Seir::u8main)
seir_target(Blocks STATIC_RUNTIME ON)

if(WIN32)
	configure_file(blocks.rc.in blocks.rc)
	target_include_directories(Blocks PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
	target_sources(Blocks PRIVATE blocks.rc.in ${CMAKE_CURRENT_BINARY_DIR}/blocks.rc)
	source_group("" FILES blocks.rc.in ${CMAKE_CURRENT_BINARY_DIR}/blocks.rc)
endif()

seir_target_pack(Blocks
	OUTPUT blocks.yp
	INDEX data/index.st
	EMBED)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT Blocks)
