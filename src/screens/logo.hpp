// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../screen.hpp"

#include <chrono>

class LogoScreen final : public Screen
{
public:
	explicit LogoScreen(Game& game) noexcept
		: Screen{ game } {}

	void start() override
	{
		_startTime = std::chrono::steady_clock::now();
	}

	void present(seir::GuiFrame&) override;

private:
	std::chrono::steady_clock::time_point _startTime;
};
