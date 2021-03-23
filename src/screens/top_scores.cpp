// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "top_scores.hpp"

#include "../game.hpp"

#include <yttrium/application/key.h>
#include <yttrium/gui/gui.h>
#include <yttrium/gui/layout.h>
#include <yttrium/renderer/2d.h>

void TopScoresScreen::present(Yt::GuiFrame& gui)
{
	_game.drawBackground(gui.renderer());
	Yt::GuiLayout layout{ gui, Yt::GuiLayout::Center{ 640, 480 } };
	layout.fromTopCenter();
	layout.skip(30);
	gui.selectBlankTexture();
	gui.renderer().setColor(Yt::Bgra32::black(0x88));
	gui.renderer().addBorderlessRect(layout.add({ 600, 250 }));
	layout.skip(30);
	if (gui.addButton("Back", "Back", layout.add({ 180, 40 })) || gui.takeKeyPress(Yt::Key::Escape))
		_game.setNextScreen(_game._mainMenuScreen);
	layout.setSize({ 0, 40 });
	layout.setSpacing(10);
	layout.fromPoint({ 50, 35 }, { 1, 1 }, Yt::GuiLayout::Axis::Y);
	for (size_t i = 0; i < _game._topScores.size(); ++i)
		gui.addLabel(std::to_string(i + 1), Yt::GuiAlignment::Right);
	layout.fromPoint({ 70, 35 }, { 1, 1 }, Yt::GuiLayout::Axis::Y);
	for (const auto& score : _game._topScores)
		gui.addLabel(score.second);
	layout.fromPoint({ 600, 35 }, { 1, 1 }, Yt::GuiLayout::Axis::Y);
	for (const auto& score : _game._topScores)
		gui.addLabel(std::to_string(score.first), Yt::GuiAlignment::Right);
}
