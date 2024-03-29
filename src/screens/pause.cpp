// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "pause.hpp"

#include "../game.hpp"

#include <yttrium/application/key.h>
#include <yttrium/gui/gui.h>
#include <yttrium/gui/layout.h>
#include <yttrium/renderer/2d.h>

void PauseScreen::present(Yt::GuiFrame& gui)
{
	_game.drawBackground(gui.renderer());
	_game.drawGraphics(gui);
	_game.drawShade(gui);
	Yt::GuiLayout layout{ gui, Yt::GuiLayout::Center{ 30, 26 } };
	layout.fromTopCenter();
	layout.skip(7.5);
	layout.setSize({ 8, 2 });
	layout.setSpacing(1);
	if (gui.addButton("Resume", "Resume") || gui.takeKeyPress(Yt::Key::Enter) || gui.takeKeyPress(Yt::Key::Escape))
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
