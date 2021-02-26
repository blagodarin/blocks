# This file is part of the Blocks game.
# Copyright (C) Sergei Blagodarin.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19)

project(Blocks LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(YTTRIUM_WITH_JPEG ON)
set(YTTRIUM_WITH_OGGVORBIS OFF)

include(FetchContent)
FetchContent_Declare(Yttrium GIT_REPOSITORY https://github.com/blagodarin/yttrium.git GIT_TAG e286b4495af80de9f43d58d3d1169bee308b0012)
FetchContent_MakeAvailable(Yttrium)

add_executable(blocks WIN32
	src/graphics.cpp
	src/graphics.hpp
	src/logic.cpp
	src/logic.hpp
	src/main.cpp
	)
target_link_libraries(blocks PRIVATE yaudio ycore yengine ymain)
set_target_properties(blocks PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
if(WIN32)
	target_sources(blocks PRIVATE blocks.rc)
endif()

file(GLOB _localization_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/data/gui/*.ion)
add_custom_target(blocks_localization
	COMMAND ytr data/i18n/en.ion data/gui.ion ${_localization_sources}
	COMMAND ytr data/i18n/ru.ion data/gui.ion ${_localization_sources}
	COMMENT "Updating localizations in ${CMAKE_CURRENT_SOURCE_DIR}/data/i18n"
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	BYPRODUCTS
		${CMAKE_CURRENT_SOURCE_DIR}/data/i18n/en.ion
		${CMAKE_CURRENT_SOURCE_DIR}/data/i18n/ru.ion
	)

yttrium_target_package(blocks
	OUTPUT blocks.ypq
	INDEX data/index.txt
	DEPENDS blocks_localization)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT blocks)
