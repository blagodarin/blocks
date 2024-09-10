// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "play_menu.hpp"

#include "../game.hpp"

#include <seir_app/events.hpp>
#include <seir_gui/frame.hpp>
#include <seir_gui/layout.hpp>

void PlayMenuScreen::present(seir::GuiFrame& gui)
{
	_game.drawBackground(gui);
	seir::GuiLayout layout{ gui, seir::GuiLayout::Center{ 30, 26 } };
	layout.fromTopCenter();
	layout.skip(1);
	layout.setItemSize({ 30, 10 });
	gui.addLabel("Blocks", seir::GuiAlignment::Center);
	layout.skip(1);
	layout.setItemSize({ 10, 2 });
	layout.setItemSpacing(1);
	if (gui.addButton("Back", "Back") || gui.takeKeyPress(seir::Key::Escape))
	{
		_game._audio->play(_game._cancelSound);
		_game.setNextScreen(_game._mainMenuScreen);
	}
	if (gui.addButton("Easy", "Easy"))
	{
		_game._audio->stopAll();
		_game._audio->play(_game._okSound);
		_game._audio->play(_game._easyGameMusic);
		_game._startLevel = 1;
		_game._logic.start(_game._startLevel);
		_game.setNextScreen(_game._gameScreen);
	}
	if (gui.addButton("Normal", "Normal"))
	{
		_game._audio->stopAll();
		_game._audio->play(_game._okSound);
		_game._audio->play(_game._normalGameMusic);
		_game._startLevel = 10;
		_game._logic.start(_game._startLevel);
		_game.setNextScreen(_game._gameScreen);
	}
	if (gui.addButton("Hard", "Hard"))
	{
		_game._audio->stopAll();
		_game._audio->play(_game._okSound);
		_game._audio->play(_game._hardGameMusic);
		_game._startLevel = 20;
		_game._logic.start(_game._startLevel);
		_game.setNextScreen(_game._gameScreen);
	}
}
