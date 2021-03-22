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

#include <yttrium/gui/gui.h>
#include <yttrium/math/rect.h>
#include <yttrium/renderer/2d.h>
#include <yttrium/renderer/texture.h>
#include <yttrium/resource_loader.h>

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
	, _backgroundTexture{ loader.load_texture_2d("data/textures/background.jpg") }
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

bool Game::present(Yt::GuiFrame& gui, const std::chrono::steady_clock::duration& duration)
{
	assert(_currentScreen);
	_currentScreen = _currentScreen->present(gui, duration);
	if (const auto cursor = gui.takeMouseCursor())
	{
		Yt::GuiLayout layout{ gui };
		gui.selectBlankTexture();
		gui.renderer().setColor(Yt::Bgra32::red());
		gui.renderer().addBorderlessRect({ *cursor, Yt::SizeF{ 10, 10 } });
	}
	return _currentScreen;
}
