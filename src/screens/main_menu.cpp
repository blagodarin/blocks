// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "main_menu.hpp"

#include "../game.hpp"

#include <yttrium/application/key.h>
#include <yttrium/gui/gui.h>
#include <yttrium/gui/layout.h>

Screen* MainMenuScreen::present(Yt::GuiFrame& gui, const std::chrono::steady_clock::duration&)
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
	Screen* next = this;
	if (gui.addButton("Play", "Play"))
		next = _game._playMenuScreen.get();
	if (gui.addButton("Top Scores", "Top Scores"))
		next = _game._topScoresScreen.get();
	if (gui.addButton("Help", "Help"))
		next = _game._helpScreen.get();
	if (gui.addButton("Exit", "Exit") || gui.captureKeyDown(Yt::Key::Escape))
		next = nullptr;
	return next;
}
