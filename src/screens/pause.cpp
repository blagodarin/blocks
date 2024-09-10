// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "pause.hpp"

#include "../game.hpp"

#include <seir_app/events.hpp>
#include <seir_gui/frame.hpp>
#include <seir_gui/layout.hpp>
#include <seir_renderer/2d.hpp>

void PauseScreen::present(seir::GuiFrame& gui)
{
	_game.drawBackground(gui);
	_game.drawGraphics(gui);
	_game.drawShade(gui);
	seir::GuiLayout layout{ gui, seir::GuiLayout::Center{ 30, 26 } };
	layout.fromTopCenter();
	layout.skip(7.5);
	layout.setItemSize({ 8, 2 });
	layout.setItemSpacing(1);
	if (gui.addButton("Resume", "Resume") || gui.takeKeyPress(seir::Key::Enter) || gui.takeKeyPress(seir::Key::Escape))
	{
		_game._logic.resume();
		_game.setNextScreen(_game._gameScreen);
	}
	if (gui.addButton("Give up", "Give up"))
	{
		_game._audio->play(_game._okSound);
		_game.setNextScreen(_game._mainMenuScreen);
	}
	if (gui.addButton("Restart", "Restart"))
	{
		_game._audio->play(_game._okSound);
		_game._logic.start(_game._startLevel);
		_game.setNextScreen(_game._gameScreen);
	}
	if (gui.addButton("Exit", "Exit"))
		_game.setNextScreen({});
}
