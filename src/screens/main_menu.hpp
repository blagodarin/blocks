// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../screen.hpp"

class MainMenuScreen final : public Screen
{
public:
	explicit MainMenuScreen(Game& game) noexcept
		: Screen{ game } {}

	void start() override;
	void present(Yt::GuiFrame&) override;
};
