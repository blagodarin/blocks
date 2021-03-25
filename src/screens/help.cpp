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
	Yt::GuiLayout layout{ gui, Yt::GuiLayout::Center{ 640, 480 } };
	layout.fromTopCenter();
	layout.skip(30);
	gui.selectBlankTexture();
	gui.renderer().setColor(Yt::Bgra32::black(0x88));
	gui.renderer().addBorderlessRect(layout.add({ 400, 300 }));
	layout.skip(30);
	Screen* next = this;
	if (gui.addButton("Back", "Back", layout.add({ 180, 40 })) || gui.takeKeyPress(Yt::Key::Escape))
	{
		_game._audio->play_sound(_game._cancelSound);
		_game.setNextScreen(_game._mainMenuScreen);
	}
	layout.setSize({ 180, 40 });
	layout.setSpacing(10);
	layout.fromPoint({ 120, 35 }, { 1, 1 }, Yt::GuiLayout::Axis::Y);
	gui.addLabel("[A]", Yt::GuiAlignment::Center);
	gui.addLabel("[D]", Yt::GuiAlignment::Center);
	gui.addLabel("[S]", Yt::GuiAlignment::Center);
	gui.addLabel("[Left]", Yt::GuiAlignment::Center);
	gui.addLabel("[Right]", Yt::GuiAlignment::Center);
	gui.addLabel("[Esc]", Yt::GuiAlignment::Center);
	layout.fromPoint({ 300, 35 }, { 1, 1 }, Yt::GuiLayout::Axis::Y);
	gui.addLabel("Move left");
	gui.addLabel("Move right");
	gui.addLabel("Accelerate");
	gui.addLabel("Rotate left");
	gui.addLabel("Rotate right");
	gui.addLabel("Pause / Exit");
}
