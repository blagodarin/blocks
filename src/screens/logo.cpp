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

void LogoScreen::present(Yt::GuiFrame& gui)
{
	constexpr std::chrono::steady_clock::duration maxDuration{ std::chrono::seconds{ 2 } };
	const auto duration = std::min(std::chrono::steady_clock::now() - _startTime, maxDuration);
	gui.selectBlankTexture();
	gui.renderer().setColor(seir::Rgba32::black());
	gui.renderer().addBorderlessRect(seir::RectF{ gui.renderer().viewportSize() });
	Yt::GuiLabelStyle style;
	style._fontSize = 1;
	const auto value = std::sin(std::numbers::pi * static_cast<double>(duration.count()) / static_cast<double>(maxDuration.count()));
	style._textColor = seir::Rgba32::white(static_cast<uint8_t>(std::lround(255 * value * value)));
	gui.setLabelStyle(style);
	Yt::GuiLayout layout{ gui, Yt::GuiLayout::Height{ 26 } };
	layout.fromTopCenter();
	layout.skip(12);
	layout.setSize({ 0, 2 });
	gui.addLabel("Powered by Yttrium", Yt::GuiAlignment::Center);
	if (gui.takeAnyKeyPress() || duration == maxDuration)
		_game.setNextScreen(_game._mainMenuScreen);
	gui.takeMouseCursor();
}
