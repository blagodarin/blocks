// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "help.hpp"

#include "../game.hpp"

#include <seir_app/events.hpp>
#include <seir_graphics/color.hpp>
#include <seir_gui/frame.hpp>
#include <seir_gui/layout.hpp>
#include <seir_renderer/2d.hpp>

void HelpScreen::present(seir::GuiFrame& gui)
{
	_game.drawBackground(gui);
	seir::GuiLayout layout{ gui, seir::GuiLayout::Center{ 30, 26 } };
	layout.fromTopCenter();
	layout.skip(1);
	gui.selectWhiteTexture();
	gui.renderer().setColor(seir::Rgba32::black(0x88));
	gui.renderer().addRect(layout.addItem({ 20, 16 }));
	layout.skip(1);
	layout.setItemSize({ 10, 2 });
	if (gui.addButton("Back", "Back") || gui.takeKeyPress(seir::Key::Escape))
	{
		_game._audio->play(_game._cancelSound);
		_game.setNextScreen(_game._mainMenuScreen);
	}
	layout.setItemSpacing(0);
	layout.setItemSize({ 6, 2 });
	layout.fromPoint({ 6, 2 }, { 1, 1 }, seir::GuiLayout::Axis::Y);
	gui.addLabel("A", seir::GuiAlignment::Center);
	gui.addLabel("D", seir::GuiAlignment::Center);
	gui.addLabel("S", seir::GuiAlignment::Center);
	gui.addLabel("\xe2\x97\x80", seir::GuiAlignment::Center);
	gui.addLabel("\xe2\x96\xb6", seir::GuiAlignment::Center);
	gui.addLabel("F10", seir::GuiAlignment::Center);
	gui.addLabel("Esc", seir::GuiAlignment::Center);
	layout.setItemSize({ 12, 2 });
	layout.fromPoint({ 12, 2 }, { 1, 1 }, seir::GuiLayout::Axis::Y);
	gui.addLabel("Move left");
	gui.addLabel("Move right");
	gui.addLabel("Accelerate");
	gui.addLabel("Rotate left");
	gui.addLabel("Rotate right");
	gui.addLabel("Take screenshot");
	gui.addLabel("Pause / Exit");
}
