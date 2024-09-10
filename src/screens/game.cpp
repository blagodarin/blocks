// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "game.hpp"

#include "../game.hpp"

#include <seir_app/events.hpp>
#include <seir_gui/frame.hpp>
#include <seir_math/vec.hpp>
#include <seir_renderer/2d.hpp>

void GameScreen::start()
{
	if (!_game._logic.is_active())
		_game._logic.start(_game._startLevel);
	_clock.reset();
}

void GameScreen::present(seir::GuiFrame& gui)
{
	_game._logic.advance(_clock.advance());
	if (!_game._logic.has_finished())
	{
		if (gui.takeKeyPress(seir::Key::Left))
			_game._logic.turn_left();
		if (gui.takeKeyPress(seir::Key::Right))
			_game._logic.turn_right();
		if (const auto state = gui.takeKeyState(seir::Key::A))
			_game._logic.set_left_movement(*state);
		if (const auto state = gui.takeKeyState(seir::Key::D))
			_game._logic.set_right_movement(*state);
		if (const auto state = gui.takeKeyState(seir::Key::S))
			_game._logic.set_acceleration(*state);
		if (gui.takeKeyPress(seir::Key::Escape))
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
	_game.drawBackground(gui);
	_game.drawGraphics(gui);
	gui.takeMouseCursor();
}
