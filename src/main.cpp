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
#include <yttrium/main.h>
#include <yttrium/renderer/2d.h>
#include <yttrium/renderer/pass.h>
#include <yttrium/renderer/viewport.h>
#include <yttrium/storage/source.h>
#include <yttrium/storage/storage.h>
#include <yttrium/storage/writer.h>

namespace
{
	constexpr unsigned char kPackage[]{
#include "blocks.ypq.inc"
	};
}

int ymain(int, char**)
{
	Yt::Logger logger;
	Yt::Storage storage{ Yt::Storage::UseFileSystem::Never };
	storage.attach_package(Yt::Source::from(kPackage, sizeof kPackage));
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
	Game game{ storage, viewport.render_manager() };
	window.show();
	while (application.process_events(gui.eventCallbacks()))
	{
		Yt::GuiFrame frame{ gui, rendered2d };
		const bool screenshot = frame.takeKeyPress(Yt::Key::F10); // Before Press-Any-Key can steal it.
		if (!game.present(frame))
			break;
		viewport.render([&rendered2d](Yt::RenderPass& pass) {
			rendered2d.draw(pass);
		});
		if (screenshot)
			viewport.take_screenshot().save_as_screenshot(Yt::ImageFormat::Png);
	}
	return 0;
}
