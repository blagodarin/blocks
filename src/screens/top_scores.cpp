// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "top_scores.hpp"

#include "../game.hpp"

#include <yttrium/application/key.h>
#include <yttrium/gui/gui.h>
#include <yttrium/gui/layout.h>
#include <yttrium/renderer/2d.h>

#include <seir_graphics/color.hpp>

void TopScoresScreen::present(Yt::GuiFrame& gui)
{
	_game.drawBackground(gui.renderer());
	Yt::GuiLayout layout{ gui, Yt::GuiLayout::Center{ 30, 26 } };
	layout.fromTopCenter();
	layout.skip(1);
	gui.selectBlankTexture();
	gui.renderer().setColor(seir::Rgba32::black(0x88));
	gui.renderer().addBorderlessRect(layout.add({ 28, 13 }));
	layout.skip(1);
	if (gui.addButton("Back", "Back", layout.add({ 10, 2 })) || gui.takeKeyPress(Yt::Key::Enter) || gui.takeKeyPress(Yt::Key::Escape))
	{
		_game._audio->play(_game._cancelSound);
		_game.setNextScreen(_game._mainMenuScreen);
	}
	layout.setSpacing(0.5);
	layout.setSize({ 2, 2 });
	layout.fromPoint({ 2, 1.5 }, { 1, 1 }, Yt::GuiLayout::Axis::Y);
	for (size_t i = 0; i < _game._topScores.size(); ++i)
		gui.addLabel(std::to_string(i + 1), Yt::GuiAlignment::Left);
	layout.setSize({ 20, 2 });
	layout.fromPoint({ 3.5, 1.5 }, { 1, 1 }, Yt::GuiLayout::Axis::Y);
	for (const auto& score : _game._topScores)
		gui.addLabel(score.second);
	layout.setSize({ 5, 2 });
	layout.fromPoint({ 23, 1.5 }, { 1, 1 }, Yt::GuiLayout::Axis::Y);
	for (const auto& score : _game._topScores)
		gui.addLabel(std::to_string(score.first), Yt::GuiAlignment::Right);
}
