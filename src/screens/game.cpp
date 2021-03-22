// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "game.hpp"

#include "../game.hpp"

#include <yttrium/application/key.h>
#include <yttrium/gui/gui.h>

Screen* GameScreen::present(Yt::GuiFrame& gui, const std::chrono::steady_clock::duration&)
{
	_game.drawBackground(gui.renderer());
	Screen* next = this;
	if (gui.captureKeyDown(Yt::Key::Escape))
		next = _game._pauseScreen.get();
	gui.takeMouseCursor();
	return next;
}
