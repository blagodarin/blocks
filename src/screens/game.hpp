// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../screen.hpp"

#include <yttrium/utils/clock.h>

class GameScreen final : public Screen
{
public:
	explicit GameScreen(Game& game) noexcept
		: Screen{ game } {}

	void start() override;
	void present(Yt::GuiFrame&) override;

private:
	Yt::Clock _clock;
};
