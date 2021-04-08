// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "main_menu.hpp"

#include "../game.hpp"

#include <yttrium/application/key.h>
#include <yttrium/gui/gui.h>
#include <yttrium/gui/layout.h>

void MainMenuScreen::start()
{
	_game._audio->play_music(_game._menuMusic);
}

void MainMenuScreen::present(Yt::GuiFrame& gui)
{
	_game.drawBackground(gui.renderer());
	Yt::GuiLayout layout{ gui, Yt::GuiLayout::Center{ 30, 26 } };
	layout.fromTopCenter();
	layout.skip(1);
	layout.setSize({ 30, 10 });
	gui.addLabel("Blocks", Yt::GuiAlignment::Center);
	layout.skip(1);
	layout.setSize({ 10, 2 });
	layout.setSpacing(1);
	if (gui.addButton("Play", "Play") || gui.takeKeyPress(Yt::Key::Enter))
	{
		_game._audio->play_sound(_game._okSound);
		_game.setNextScreen(_game._playMenuScreen);
	}
	if (gui.addButton("Top Scores", "Top Scores"))
	{
		_game._audio->play_sound(_game._okSound);
		_game.setNextScreen(_game._topScoresScreen);
	}
	if (gui.addButton("Controls", "Controls"))
	{
		_game._audio->play_sound(_game._okSound);
		_game.setNextScreen(_game._helpScreen);
	}
	if (gui.addButton("Exit", "Exit") || gui.takeKeyPress(Yt::Key::Escape))
	{
		_game._audio->play_sound(_game._cancelSound);
		_game.setNextScreen({});
	}
}
