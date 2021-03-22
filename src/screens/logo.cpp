// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "logo.hpp"

#include "../game.hpp"

#include <yttrium/application/key.h>
#include <yttrium/gui/gui.h>
#include <yttrium/gui/layout.h>
#include <yttrium/gui/style.h>
#include <yttrium/renderer/2d.h>

#include <numbers>

Screen* LogoScreen::present(Yt::GuiFrame& gui, const std::chrono::steady_clock::duration& duration)
{
	constexpr std::chrono::steady_clock::duration screenTime{ std::chrono::milliseconds{ 3000 } };
	_time = std::min(_time + duration, screenTime);
	gui.selectBlankTexture();
	gui.renderer().setColor(Yt::Bgra32::black());
	gui.renderer().addBorderlessRect(Yt::RectF{ gui.renderer().viewportSize() });
	Yt::GuiLabelStyle style;
	style._fontSize = 1;
	const auto value = std::sin(std::numbers::pi * static_cast<double>(_time.count()) / static_cast<double>(screenTime.count()));
	style._textColor = Yt::Bgra32::white(static_cast<uint8_t>(std::lround(255 * value * value)));
	gui.setLabelStyle(style);
	Yt::GuiLayout layout{ gui, Yt::GuiLayout::Height{ 64 } };
	layout.fromTopCenter();
	layout.skip(24);
	layout.setSize({ 0, 8 });
	gui.addLabel("Blocks", Yt::GuiAlignment::Center);
	layout.setSize({ 0, 4 });
	gui.addLabel("by Yttrium", Yt::GuiAlignment::Center);
	gui.takeMouseCursor();
	return _time < screenTime && !gui.captureKeyDown(Yt::Key::Escape) ? this : _game._mainMenuScreen.get();
}
