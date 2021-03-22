// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../screen.hpp"

class TopScoresScreen final : public Screen
{
public:
	explicit TopScoresScreen(Game& game) noexcept
		: Screen{ game } {}

	Screen* present(Yt::GuiFrame&, const std::chrono::steady_clock::duration& duration) override;
};
