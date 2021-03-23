// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../screen.hpp"

#include <chrono>

class GameScreen final : public Screen
{
public:
	explicit GameScreen(Game& game) noexcept
		: Screen{ game } {}

	void start() override;
	void present(Yt::GuiFrame&) override;

private:
	std::chrono::steady_clock::time_point _lastTime;
};
