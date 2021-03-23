// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "main_menu.hpp"

#include "../game.hpp"

#include <yttrium/application/key.h>
#include <yttrium/gui/gui.h>
#include <yttrium/gui/layout.h>

void MainMenuScreen::present(Yt::GuiFrame& gui)
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
	if (gui.addButton("Play", "Play"))
		_game.setNextScreen(_game._playMenuScreen);
	if (gui.addButton("Top Scores", "Top Scores"))
		_game.setNextScreen(_game._topScoresScreen);
	if (gui.addButton("Help", "Help"))
		_game.setNextScreen(_game._helpScreen);
	if (gui.addButton("Exit", "Exit") || gui.takeKeyPress(Yt::Key::Escape))
		_game.setNextScreen({});
}
