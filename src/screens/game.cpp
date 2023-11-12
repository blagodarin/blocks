// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "game.hpp"

#include "../game.hpp"

#include <yttrium/application/key.h>
#include <yttrium/gui/gui.h>
#include <yttrium/renderer/2d.h>

#include <cassert>

void GameScreen::start()
{
	if (!_game._logic.is_active())
		_game._logic.start(_game._startLevel);
	_clock = Yt::Clock{};
}

void GameScreen::present(Yt::GuiFrame& gui)
{
	_game._logic.advance(_clock.tick());
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
	{
		_game._audio->stopAll();
		_game._audio->play(_game._gameOverMusic);
		_game.setNextScreen(_game._logic.score() > _game._topScores.back().first ? _game._gameOverScreen : _game._topScoresScreen);
	}
	_game.drawBackground(gui.renderer());
	_game.drawGraphics(gui);
	gui.takeMouseCursor();
}
