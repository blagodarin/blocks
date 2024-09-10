// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "graphics.hpp"

#include "game.hpp"

#include <seir_app/app.hpp>
#include <seir_app/events.hpp>
#include <seir_app/window.hpp>
#include <seir_data/blob.hpp>
#include <seir_data/storage.hpp>
#include <seir_gui/context.hpp>
#include <seir_gui/font.hpp>
#include <seir_gui/frame.hpp>
#include <seir_image/image.hpp>
#include <seir_renderer/2d.hpp>
#include <seir_renderer/renderer.hpp>
#include <seir_u8main/u8main.hpp>

//#include <future>

namespace
{
	constexpr unsigned char kPackage[]{
#include "blocks.yp.inc"
	};
}

int u8main(int, char**)
{
	seir::Storage storage{ seir::Storage::UseFileSystem::Never };
	storage.attachArchive(seir::Blob::from(kPackage, sizeof kPackage));
	seir::App app;
	seir::Window window{ app, "Blocks" };
	if (const auto icon = seir::Image::load(storage.open("data/icon.ico")))
		window.setIcon(*icon);
	seir::Renderer renderer{ window };
	seir::Renderer2D renderer2d;
	seir::GuiContext gui{ window };
	gui.setDefaultFont(seir::Font::load(renderer, storage.open("data/fonts/source_sans_pro.ttf"), 64));
	Game game{ storage, renderer };
	//std::future<void> screenshotFuture;
	window.show();
	while (app.processEvents(gui.eventCallbacks()))
	{
		seir::GuiFrame frame{ gui, renderer2d };
		//const bool screenshot = frame.takeKeyPress(seir::Key::F10); // Before Press-Any-Key can steal it.
		if (!game.present(frame))
			break;
		renderer.render([&renderer2d](seir::RenderPass& pass) {
			renderer2d.draw(pass);
		});
		//if (screenshot)
		//	screenshotFuture = std::async(std::launch::async, [image = viewport.take_screenshot()] { image.saveAsScreenshot(seir::ImageFormat::Png, 100); });
	}
	return 0;
}
