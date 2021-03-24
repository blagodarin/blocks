// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "game.hpp"

#include "screens/game.hpp"
#include "screens/game_over.hpp"
#include "screens/help.hpp"
#include "screens/logo.hpp"
#include "screens/main_menu.hpp"
#include "screens/pause.hpp"
#include "screens/play_menu.hpp"
#include "screens/top_scores.hpp"

#include <yttrium/geometry/rect.h>
#include <yttrium/gui/gui.h>
#include <yttrium/renderer/2d.h>
#include <yttrium/renderer/resource_loader.h>
#include <yttrium/renderer/texture.h>

#include <cassert>

namespace
{
	Yt::RectF scaleToFill(const Yt::SizeF& textureSize, const Yt::SizeF& rectSize)
	{
		const auto widthRatio = rectSize._width / textureSize._width;
		const auto heightRatio = rectSize._height / textureSize._height;
		if (widthRatio > heightRatio)
		{
			const auto offsetY = rectSize._height / (2 * widthRatio);
			return Yt::RectF{ { 0, offsetY }, Yt::Vector2{ textureSize._width, textureSize._height - offsetY } };
		}
		const auto offsetX = rectSize._width / (2 * heightRatio);
		return Yt::RectF{ { offsetX, 0 }, Yt::Vector2{ textureSize._width - offsetX, textureSize._height } };
	}
}

Game::Game(Yt::ResourceLoader& loader)
	: _logoScreen{ std::make_unique<LogoScreen>(*this) }
	, _mainMenuScreen{ std::make_unique<MainMenuScreen>(*this) }
	, _playMenuScreen{ std::make_unique<PlayMenuScreen>(*this) }
	, _gameScreen{ std::make_unique<GameScreen>(*this) }
	, _pauseScreen{ std::make_unique<PauseScreen>(*this) }
	, _gameOverScreen{ std::make_unique<GameOverScreen>(*this) }
	, _topScoresScreen{ std::make_unique<TopScoresScreen>(*this) }
	, _helpScreen{ std::make_unique<HelpScreen>(*this) }
	, _graphics{ loader.render_manager() }
	, _backgroundTexture{ loader.load_texture_2d("data/textures/background.jpg") }
	, _cursorTexture{ loader.load_texture_2d("data/textures/cursor.tga") }
{
	_topScores.emplace_back(100'000, "Grandmaster");
	_topScores.emplace_back(80'000, "Master");
	_topScores.emplace_back(50'000, "Professional");
	_topScores.emplace_back(20'000, "Amateur");
	_topScores.emplace_back(10'000, "Novice");
}

Game::~Game() = default;

void Game::drawBackground(Yt::Renderer2D& renderer) const
{
	renderer.setTexture(_backgroundTexture);
	renderer.setTextureRect(::scaleToFill(Yt::SizeF{ _backgroundTexture->size() }, renderer.viewportSize()));
	renderer.setColor(Yt::Bgra32::white());
	renderer.addBorderlessRect(Yt::RectF{ renderer.viewportSize() });
}

void Game::drawGraphics(Yt::GuiFrame& gui) const
{
	Yt::GuiLayout layout{ gui, Yt::GuiLayout::Center{ 30, 26 } };
	gui.selectBlankTexture();
	gui.renderer().setColor(Yt::Bgra32::black(0x88));
	gui.renderer().addRect(layout.map({ { 1, 2 }, Yt::SizeF{ 6, 5 } }));
	gui.renderer().addRect(layout.map({ { 10, 2 }, Yt::SizeF{ 10, 22 } }));
	gui.renderer().addRect(layout.map({ { 23, 2 }, Yt::SizeF{ 6, 2 } }));
	gui.renderer().addRect(layout.map({ { 23, 6 }, Yt::SizeF{ 6, 2 } }));
	gui.renderer().addRect(layout.map({ { 23, 10 }, Yt::SizeF{ 6, 2 } }));
	gui.addLabel("Next:", Yt::GuiAlignment::Left, layout.map({ { 1.5, 2 }, Yt::SizeF{ 5, 1 } }));
	gui.addLabel("Level:", Yt::GuiAlignment::Left, layout.map({ { 23.5, 2 }, Yt::SizeF{ 5, 1 } }));
	gui.addLabel(std::to_string(_logic.level()), Yt::GuiAlignment::Right, layout.map({ { 23.5, 3 }, Yt::SizeF{ 5, 1 } }));
	gui.addLabel("Lines:", Yt::GuiAlignment::Left, layout.map({ { 23.5, 6 }, Yt::SizeF{ 5, 1 } }));
	gui.addLabel(std::to_string(_logic.lines()), Yt::GuiAlignment::Right, layout.map({ { 23.5, 7 }, Yt::SizeF{ 5, 1 } }));
	gui.addLabel("Score:", Yt::GuiAlignment::Left, layout.map({ { 23.5, 10 }, Yt::SizeF{ 5, 1 } }));
	gui.addLabel(std::to_string(_logic.score()), Yt::GuiAlignment::Right, layout.map({ { 23.5, 11 }, Yt::SizeF{ 5, 1 } }));
	_graphics.drawField(gui.renderer(), layout.map({ { 9, 1 }, Yt::SizeF{ 12, 24 } }), _logic.field(), _logic.current_figure());
	_graphics.drawNextFigure(gui.renderer(), layout.map({ { 2, 4 }, Yt::SizeF{ 4, 2 } }), _logic.next_figure());
}

void Game::drawShade(Yt::GuiFrame& gui) const
{
	gui.selectBlankTexture();
	gui.renderer().setColor(Yt::Bgra32::black(0x88));
	gui.renderer().addRect(Yt::RectF{ gui.renderer().viewportSize() });
}

void Game::setNextScreen(const std::unique_ptr<Screen>& screen)
{
	_nextScreen = screen.get();
}

bool Game::present(Yt::GuiFrame& gui)
{
	if (_currentScreen != _nextScreen)
	{
		_currentScreen = _nextScreen;
		_currentScreen->start();
	}
	assert(_currentScreen);
	_currentScreen->present(gui);
	if (const auto cursor = gui.takeMouseCursor())
	{
		gui.renderer().setTexture(_cursorTexture);
		gui.renderer().setColor(Yt::Bgra32::white());
		gui.renderer().addBorderlessRect({ *cursor, Yt::SizeF{ _cursorTexture->size() } });
	}
	return _nextScreen;
}
