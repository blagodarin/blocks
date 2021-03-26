// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "graphics.hpp"

#include "game.hpp"

#include <yttrium/application/application.h>
#include <yttrium/application/key.h>
#include <yttrium/application/window.h>
#include <yttrium/audio/format.h>
#include <yttrium/audio/manager.h>
#include <yttrium/audio/utils.h>
#include <yttrium/base/logger.h>
#include <yttrium/gui/context.h>
#include <yttrium/gui/font.h>
#include <yttrium/gui/gui.h>
#include <yttrium/image/image.h>
#include <yttrium/image/utils.h>
#include <yttrium/main.h>
#include <yttrium/renderer/2d.h>
#include <yttrium/renderer/pass.h>
#include <yttrium/renderer/resource_loader.h>
#include <yttrium/renderer/viewport.h>
#include <yttrium/storage/source.h>
#include <yttrium/storage/storage.h>
#include <yttrium/storage/writer.h>

#include <cassert>
#include <numbers>

namespace
{
	void makeButtonsTexture(Yt::Storage& storage, const std::string& name, size_t button_size)
	{
		constexpr size_t button_styles = 4;
		storage.attach_buffer(name, Yt::make_bgra32_tga(button_size, button_size * button_styles, [button_size](size_t, size_t y) {
			const auto style = y / button_size;
			return Yt::Bgra32{ 0xff, 0x44 * style, 0x44 * style };
		}));
	}

	void makeCursorTexture(Yt::Storage& storage, const std::string& name, size_t size)
	{
		storage.attach_buffer(name, Yt::make_bgra32_tga(size, size, [size](size_t x, size_t y) {
			if (y > 2 * x || 2 * y < x || (y > 2 * (size - x) && x > 2 * (size - y)))
				return Yt::Bgra32{ 0, 0, 0, 0 };
			else
				return Yt::Bgra32{
					y * 0xff / (size - 1),
					x * 0xff / (size - 1),
					(size * size - x * y) * 0xff / (size * size),
				};
		}));
	}
}

int ymain(int, char**)
{
	Yt::Logger logger;
	Yt::Storage storage{ Yt::Storage::UseFileSystem::Never };
	storage.attach_package("blocks.ypq");
	::makeButtonsTexture(storage, "data/textures/buttons.tga", 16);
	::makeCursorTexture(storage, "data/textures/cursor.tga", 64);
	Yt::Application application;
	Yt::Window window{ application, "Blocks" };
	if (const auto iconSource = storage.open("data/icon.ico"))
		if (const auto iconImage = Yt::Image::load(*iconSource))
			window.set_icon(*iconImage);
	Yt::Viewport viewport{ window };
	Yt::GuiContext gui{ window };
	if (const auto fontSource = storage.open("data/fonts/source_sans_pro.ttf"))
		gui.setDefaultFont(Yt::Font::load(*fontSource, viewport.render_manager()));
	Yt::Renderer2D rendered2d{ viewport };
	Yt::ResourceLoader resourceLoader{ storage, viewport.render_manager() };
	Game game{ storage, resourceLoader };
	window.show();
	while (application.process_events(gui.eventCallbacks()))
	{
		Yt::GuiFrame guiFrame{ gui, rendered2d };
		if (!game.present(guiFrame))
			break;
		viewport.render([&rendered2d](Yt::RenderPass& pass) {
			rendered2d.draw(pass);
		});
		if (guiFrame.takeKeyPress(Yt::Key::F10))
			viewport.take_screenshot().save_as_screenshot(Yt::ImageFormat::Jpeg, 90);
	}
	return 0;
}
