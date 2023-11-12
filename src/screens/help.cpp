// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "help.hpp"

#include "../game.hpp"

#include <yttrium/application/key.h>
#include <yttrium/gui/gui.h>
#include <yttrium/gui/layout.h>
#include <yttrium/renderer/2d.h>

void HelpScreen::present(Yt::GuiFrame& gui)
{
	_game.drawBackground(gui.renderer());
	Yt::GuiLayout layout{ gui, Yt::GuiLayout::Center{ 30, 26 } };
	layout.fromTopCenter();
	layout.skip(1);
	gui.selectBlankTexture();
	gui.renderer().setColor(Yt::Bgra32::black(0x88));
	gui.renderer().addBorderlessRect(layout.add({ 20, 16 }));
	layout.skip(1);
	if (gui.addButton("Back", "Back", layout.add({ 10, 2 })) || gui.takeKeyPress(Yt::Key::Escape))
	{
		_game._audio->play(_game._cancelSound);
		_game.setNextScreen(_game._mainMenuScreen);
	}
	layout.setSpacing(0);
	layout.setSize({ 6, 2 });
	layout.fromPoint({ 6, 2 }, { 1, 1 }, Yt::GuiLayout::Axis::Y);
	gui.addLabel("A", Yt::GuiAlignment::Center);
	gui.addLabel("D", Yt::GuiAlignment::Center);
	gui.addLabel("S", Yt::GuiAlignment::Center);
	gui.addLabel("\xe2\x97\x80", Yt::GuiAlignment::Center);
	gui.addLabel("\xe2\x96\xb6", Yt::GuiAlignment::Center);
	gui.addLabel("F10", Yt::GuiAlignment::Center);
	gui.addLabel("Esc", Yt::GuiAlignment::Center);
	layout.setSize({ 12, 2 });
	layout.fromPoint({ 12, 2 }, { 1, 1 }, Yt::GuiLayout::Axis::Y);
	gui.addLabel("Move left");
	gui.addLabel("Move right");
	gui.addLabel("Accelerate");
	gui.addLabel("Rotate left");
	gui.addLabel("Rotate right");
	gui.addLabel("Take screenshot");
	gui.addLabel("Pause / Exit");
}
