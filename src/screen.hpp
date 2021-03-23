// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#pragma once

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
	virtual void start() {}
	virtual void present(Yt::GuiFrame&) = 0;

protected:
	Game& _game;
};
