// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "game_over.hpp"

#include "../game.hpp"

#include <seir_app/events.hpp>
#include <seir_base/string_utils.hpp>
#include <seir_gui/frame.hpp>
#include <seir_gui/layout.hpp>
#include <seir_renderer/2d.hpp>

void GameOverScreen::present(seir::GuiFrame& gui)
{
	_game.drawBackground(gui);
	_game.drawGraphics(gui);
	_game.drawShade(gui);
	seir::GuiLayout layout{ gui, seir::GuiLayout::Center{ 30, 26 } };
	layout.fromTopCenter();
	layout.skip(11);
	layout.setItemSize({ 0, 2 });
	gui.addLabel("Enter your name:", seir::GuiAlignment::Center);
	layout.skip(0.5);
	if (std::exchange(_justStarted, false))
		gui.putKeyboardFocus();
	layout.setItemSize({ 14, 2 });
	const bool nameEntered = gui.addStringEdit("Name", _name);
	seir::normalizeWhitespace(_name, nameEntered ? seir::TrailingSpace::Remove : seir::TrailingSpace::Keep);
	if (nameEntered && !_name.empty())
	{
		_game._topScores.emplace_back(_game._logic.score(), _name);
		std::stable_sort(_game._topScores.begin(), _game._topScores.end(), [](const auto& left, const auto& right) { return left.first > right.first; });
		_game._topScores.pop_back();
		_game.setNextScreen(_game._topScoresScreen);
	}
	if (gui.takeKeyPress(seir::Key::Escape))
		_game.setNextScreen(_game._mainMenuScreen);
}
