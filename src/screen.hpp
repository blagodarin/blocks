// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <chrono>

namespace Yt
{
	class GuiFrame;
}

class Game;

class Screen
{
public:
	explicit Screen(Game& game) noexcept
		: _game{ game } {}

	virtual ~Screen() noexcept = default;
	virtual Screen* present(Yt::GuiFrame&, const std::chrono::steady_clock::duration&) = 0;

protected:
	Game& _game;
};
