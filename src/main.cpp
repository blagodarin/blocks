// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "graphics.hpp"

#include "game.hpp"

#include <yttrium/application/application.h>
#include <yttrium/application/key.h>
#include <yttrium/application/window.h>
#include <yttrium/base/logger.h>
#include <yttrium/gui/context.h>
#include <yttrium/gui/font.h>
#include <yttrium/gui/gui.h>
#include <yttrium/image/image.h>
#include <yttrium/renderer/2d.h>
#include <yttrium/renderer/pass.h>
#include <yttrium/renderer/viewport.h>
#include <yttrium/storage/source.h>
#include <yttrium/storage/storage.h>
#include <yttrium/storage/writer.h>

#include <seir_data/blob.hpp>
#include <seir_data/storage.hpp>
#include <seir_image/image.hpp>
#include <seir_u8main/u8main.hpp>

#include <future>

namespace
{
	constexpr unsigned char kPackage[]{
#include "blocks.yp.inc"
	};
}

int u8main(int, char**)
{
	Yt::Logger logger;
	seir::Storage storage{ seir::Storage::UseFileSystem::Never };
	storage.attachArchive(seir::Blob::from(kPackage, sizeof kPackage));
	Yt::Application application;
	Yt::Window window{ application, "Blocks" };
	if (const auto icon = seir::Image::load(storage.open("data/icon.ico")))
		window.set_icon(*icon);
	Yt::Viewport viewport{ window };
	Yt::GuiContext gui{ window };
	gui.setDefaultFont(Yt::Font::load(storage.open("data/fonts/source_sans_pro.ttf"), viewport.render_manager()));
	Yt::Renderer2D rendered2d{ viewport };
	Game game{ storage, viewport.render_manager() };
	std::future<void> screenshotFuture;
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
			screenshotFuture = std::async(std::launch::async, [image = viewport.take_screenshot()] { image.save_as_screenshot(Yt::ImageFormat::Png, 100); });
	}
	return 0;
}
