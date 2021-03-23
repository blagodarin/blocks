// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "game.hpp"

#include "../game.hpp"

#include <yttrium/application/key.h>
#include <yttrium/gui/gui.h>
#include <yttrium/math/color.h>
#include <yttrium/renderer/2d.h>

#include <cassert>

void GameScreen::start()
{
	if (!_game._logic.is_active())
		_game._logic.start(_game._startLevel);
	_lastTime = std::chrono::steady_clock::now();
}

void GameScreen::present(Yt::GuiFrame& gui)
{
	const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _lastTime);
	_lastTime += duration;
	_game._logic.advance(static_cast<int>(duration.count()));
	if (!_game._logic.has_finished())
	{
		if (gui.takeKeyPress(Yt::Key::Left))
			_game._logic.turn_left();
		if (gui.takeKeyPress(Yt::Key::Right))
			_game._logic.turn_right();
		if (const auto state = gui.takeKeyState(Yt::Key::A))
			_game._logic.set_left_movement(*state);
		if (const auto state = gui.takeKeyState(Yt::Key::D))
			_game._logic.set_right_movement(*state);
		if (const auto state = gui.takeKeyState(Yt::Key::S))
			_game._logic.set_acceleration(*state);
		if (gui.takeKeyPress(Yt::Key::Escape))
		{
			_game._logic.pause();
			_game.setNextScreen(_game._pauseScreen);
		}
	}
	else
		_game.setNextScreen(_game._gameOverScreen);
	_game.drawBackground(gui.renderer());
	_game.drawGraphics(gui);
	gui.takeMouseCursor();
}
