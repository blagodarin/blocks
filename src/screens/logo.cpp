// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "logo.hpp"

#include "../game.hpp"

#include <seir_app/events.hpp>
#include <seir_gui/frame.hpp>
#include <seir_gui/layout.hpp>
#include <seir_gui/style.hpp>
#include <seir_renderer/2d.hpp>

#include <numbers>

void LogoScreen::present(seir::GuiFrame& gui)
{
	constexpr std::chrono::steady_clock::duration maxDuration{ std::chrono::seconds{ 2 } };
	const auto duration = std::min(std::chrono::steady_clock::now() - _startTime, maxDuration);
	gui.selectWhiteTexture();
	gui.renderer().setColor(seir::Rgba32::black());
	gui.renderer().addRect(seir::RectF{ gui.size() });
	seir::GuiLabelStyle style;
	style._fontSize = 1;
	const auto value = std::sin(std::numbers::pi * static_cast<double>(duration.count()) / static_cast<double>(maxDuration.count()));
	style._textColor = seir::Rgba32::white(static_cast<uint8_t>(std::lround(255 * value * value)));
	gui.setLabelStyle(style);
	seir::GuiLayout layout{ gui, seir::GuiLayout::Height{ 26 } };
	layout.fromTopCenter();
	layout.skip(12);
	layout.setItemSize({ 0, 2 });
	gui.addLabel("Powered by Seir", seir::GuiAlignment::Center);
	if (gui.takeAnyKeyPress() || duration == maxDuration)
		_game.setNextScreen(_game._mainMenuScreen);
	gui.takeMouseCursor();
}
