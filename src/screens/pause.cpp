// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "pause.hpp"

#include "../game.hpp"

#include <yttrium/application/key.h>
#include <yttrium/gui/gui.h>
#include <yttrium/gui/layout.h>
#include <yttrium/renderer/2d.h>

Screen* PauseScreen::present(Yt::GuiFrame& gui, const std::chrono::steady_clock::duration&)
{
	_game.drawBackground(gui.renderer());
	gui.selectBlankTexture();
	gui.renderer().setColor(Yt::Bgra32::black(0x88));
	gui.renderer().addRect({ { 0, 0 }, gui.renderer().viewportSize() });
	Yt::GuiLayout layout{ gui, Yt::GuiLayout::Center{ 30, 26 } };
	layout.fromTopCenter();
	layout.skip(7);
	layout.setSize({ 8, 2 });
	layout.setSpacing(1);
	Screen* next = this;
	if (gui.addButton("Resume", "Resume") || gui.captureKeyDown(Yt::Key::Escape))
		next = _game._gameScreen.get();
	if (gui.addButton("Give Up", "Give Up"))
		next = _game._gameOverScreen.get();
	if (gui.addButton("Restart", "Restart"))
		next = _game._gameScreen.get();
	if (gui.addButton("Exit", "Exit"))
		next = nullptr;
	return next;
}
