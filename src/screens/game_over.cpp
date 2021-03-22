// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "game_over.hpp"

#include "../game.hpp"

#include <yttrium/application/key.h>
#include <yttrium/gui/gui.h>
#include <yttrium/gui/layout.h>
#include <yttrium/renderer/2d.h>

Screen* GameOverScreen::present(Yt::GuiFrame& gui, const std::chrono::steady_clock::duration&)
{
	_game.drawBackground(gui.renderer());
	gui.selectBlankTexture();
	gui.renderer().setColor(Yt::Bgra32::black(0x88));
	gui.renderer().addRect({ { 0, 0 }, gui.renderer().viewportSize() });
	Yt::GuiLayout layout{ gui, Yt::GuiLayout::Center{ 30, 26 } };
	layout.fromTopCenter();
	layout.skip(4);
	layout.setSpacing(1);
	gui.addLabel("Game Over", Yt::GuiAlignment::Center, layout.add({ 0, 8 }));
	gui.addLabel("Enter your name:", Yt::GuiAlignment::Center, layout.add({ 0, 3 }));
	Screen* next = this;
	if (gui.addStringEdit("Name", _name, layout.add({ 20, 4 })))
		next = _game._topScoresScreen.get();
	if (gui.captureKeyDown(Yt::Key::Escape))
		next = _game._mainMenuScreen.get();
	return next;
}
