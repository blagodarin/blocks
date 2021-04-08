// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "game_over.hpp"

#include "../game.hpp"

#include <yttrium/application/key.h>
#include <yttrium/base/string.h>
#include <yttrium/gui/gui.h>
#include <yttrium/gui/layout.h>
#include <yttrium/renderer/2d.h>

void GameOverScreen::present(Yt::GuiFrame& gui)
{
	_game.drawBackground(gui.renderer());
	_game.drawGraphics(gui);
	_game.drawShade(gui);
	Yt::GuiLayout layout{ gui, Yt::GuiLayout::Center{ 30, 26 } };
	layout.fromTopCenter();
	layout.skip(4);
	layout.setSpacing(1);
	gui.addLabel("Game over", Yt::GuiAlignment::Center, layout.add({ 0, 8 }));
	gui.addLabel("Enter your name:", Yt::GuiAlignment::Center, layout.add({ 0, 3 }));
	if (std::exchange(_justStarted, false))
		gui.putDefaultFocus();
	const bool nameEntered = gui.addStringEdit("Name", _name, layout.add({ 20, 4 }));
	Yt::strip(_name, nameEntered);
	if (nameEntered && !_name.empty())
	{
		_game._topScores.emplace_back(_game._logic.score(), _name);
		std::stable_sort(_game._topScores.begin(), _game._topScores.end(), [](const auto& left, const auto& right) { return left.first > right.first; });
		_game._topScores.pop_back();
		_game.setNextScreen(_game._topScoresScreen);
	}
	if (gui.takeKeyPress(Yt::Key::Escape))
		_game.setNextScreen(_game._mainMenuScreen);
}
