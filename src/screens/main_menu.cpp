// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "main_menu.hpp"

#include "../game.hpp"

#include <seir_app/events.hpp>
#include <seir_gui/frame.hpp>
#include <seir_gui/layout.hpp>

void MainMenuScreen::start()
{
	_game._audio->stopAll();
	_game._audio->play(_game._menuMusic);
}

void MainMenuScreen::present(seir::GuiFrame& gui)
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
	if (gui.addButton("Play", "Play") || gui.takeKeyPress(seir::Key::Enter))
	{
		_game._audio->play(_game._okSound);
		_game.setNextScreen(_game._playMenuScreen);
	}
	if (gui.addButton("Top Scores", "Top Scores"))
	{
		_game._audio->play(_game._okSound);
		_game.setNextScreen(_game._topScoresScreen);
	}
	if (gui.addButton("Controls", "Controls"))
	{
		_game._audio->play(_game._okSound);
		_game.setNextScreen(_game._helpScreen);
	}
	if (gui.addButton("Exit", "Exit") || gui.takeKeyPress(seir::Key::Escape))
	{
		_game._audio->play(_game._cancelSound);
		_game.setNextScreen({});
	}
}
