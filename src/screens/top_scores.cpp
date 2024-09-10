// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "top_scores.hpp"

#include "../game.hpp"

#include <seir_app/events.hpp>
#include <seir_graphics/color.hpp>
#include <seir_gui/frame.hpp>
#include <seir_gui/layout.hpp>
#include <seir_renderer/2d.hpp>

void TopScoresScreen::present(seir::GuiFrame& gui)
{
	_game.drawBackground(gui);
	seir::GuiLayout layout{ gui, seir::GuiLayout::Center{ 30, 26 } };
	layout.fromTopCenter();
	layout.skip(1);
	gui.selectWhiteTexture();
	gui.renderer().setColor(seir::Rgba32::black(0x88));
	gui.renderer().addRect(layout.addItem({ 28, 13 }));
	layout.skip(1);
	layout.setItemSize({ 10, 2 });
	if (gui.addButton("Back", "Back") || gui.takeKeyPress(seir::Key::Enter) || gui.takeKeyPress(seir::Key::Escape))
	{
		_game._audio->play(_game._cancelSound);
		_game.setNextScreen(_game._mainMenuScreen);
	}
	layout.setItemSpacing(0.5);
	layout.setItemSize({ 2, 2 });
	layout.fromPoint({ 2, 1.5 }, { 1, 1 }, seir::GuiLayout::Axis::Y);
	for (size_t i = 0; i < _game._topScores.size(); ++i)
		gui.addLabel(std::to_string(i + 1), seir::GuiAlignment::Left);
	layout.setItemSize({ 20, 2 });
	layout.fromPoint({ 3.5, 1.5 }, { 1, 1 }, seir::GuiLayout::Axis::Y);
	for (const auto& score : _game._topScores)
		gui.addLabel(score.second);
	layout.setItemSize({ 5, 2 });
	layout.fromPoint({ 23, 1.5 }, { 1, 1 }, seir::GuiLayout::Axis::Y);
	for (const auto& score : _game._topScores)
		gui.addLabel(std::to_string(score.first), seir::GuiAlignment::Right);
}
