// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../screen.hpp"

#include <seir_base/clock.hpp>

class GameScreen final : public Screen
{
public:
	explicit GameScreen(Game& game) noexcept
		: Screen{ game } {}

	void start() override;
	void present(seir::GuiFrame&) override;

private:
	seir::ConstantRate<> _clock{ std::chrono::milliseconds{ 1 } };
};
