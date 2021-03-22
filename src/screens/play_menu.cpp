// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "play_menu.hpp"

#include "../game.hpp"

#include <yttrium/application/key.h>
#include <yttrium/gui/gui.h>
#include <yttrium/gui/layout.h>

Screen* PlayMenuScreen::present(Yt::GuiFrame& gui, const std::chrono::steady_clock::duration&)
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
	if (gui.addButton("Easy", "Easy"))
		next = _game._gameScreen.get();
	if (gui.addButton("Normal", "Normal"))
		next = _game._gameScreen.get();
	if (gui.addButton("Hard", "Hard"))
		next = _game._gameScreen.get();
	if (gui.addButton("Back", "Back") || gui.captureKeyDown(Yt::Key::Escape))
		next = _game._mainMenuScreen.get();
	return next;
}
