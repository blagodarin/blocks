// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../screen.hpp"

#include <string>

class GameOverScreen final : public Screen
{
public:
	explicit GameOverScreen(Game& game) noexcept
		: Screen{ game } {}

	void present(Yt::GuiFrame&) override;

private:
	std::string _name;
};
