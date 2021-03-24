# This file is part of the Blocks game.
# Copyright (C) Sergei Blagodarin.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19)

project(Blocks LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(YTTRIUM_WITH_JPEG ON)
set(YTTRIUM_WITH_OGGVORBIS OFF)

include(FetchContent)
FetchContent_Declare(Yttrium GIT_REPOSITORY https://github.com/blagodarin/yttrium.git GIT_TAG f19c609390c9abce1ac8ab646c5a160a6f330138)
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
target_link_libraries(blocks PRIVATE yapplication yaudio ycore ygui yimage ymain yrenderer)
set_target_properties(blocks PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

yttrium_target_package(blocks
	OUTPUT blocks.ypq
	INDEX data/index.txt)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT blocks)
