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

	void start() override { _justStarted = true; }
	void present(seir::GuiFrame&) override;

private:
	bool _justStarted = false;
	std::string _name = "Player";
};
