// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "play_menu.hpp"

#include "../game.hpp"

#include <yttrium/application/key.h>
#include <yttrium/gui/gui.h>
#include <yttrium/gui/layout.h>

void PlayMenuScreen::present(Yt::GuiFrame& gui)
{
	_game.drawBackground(gui.renderer());
	Yt::GuiLayout layout{ gui, Yt::GuiLayout::Center{ 640, 480 } };
	layout.fromTopCenter();
	layout.skip(20);
	layout.setSize({ 640, 160 });
	gui.addLabel("Blocks", Yt::GuiAlignment::Center);
	layout.skip(30);
	layout.setSize({ 180, 40 });
	layout.setSpacing(10);
	if (gui.addButton("Easy", "Easy"))
	{
		_game._audio->play_sound(_game._okSound);
		_game._audio->play_music(_game._easyGameMusic);
		_game._startLevel = 1;
		_game._logic.start(_game._startLevel);
		_game.setNextScreen(_game._gameScreen);
	}
	if (gui.addButton("Normal", "Normal"))
	{
		_game._audio->play_sound(_game._okSound);
		_game._audio->play_music(_game._normalGameMusic);
		_game._startLevel = 10;
		_game._logic.start(_game._startLevel);
		_game.setNextScreen(_game._gameScreen);
	}
	if (gui.addButton("Hard", "Hard"))
	{
		_game._audio->play_sound(_game._okSound);
		_game._audio->play_music(_game._hardGameMusic);
		_game._startLevel = 30;
		_game._logic.start(_game._startLevel);
		_game.setNextScreen(_game._gameScreen);
	}
	if (gui.addButton("Back", "Back") || gui.takeKeyPress(Yt::Key::Escape))
	{
		_game._audio->play_sound(_game._cancelSound);
		_game.setNextScreen(_game._mainMenuScreen);
	}
}
