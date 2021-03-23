// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "graphics.hpp"
#include "logic.hpp"

#include <chrono>
#include <memory>
#include <string>
#include <vector>

namespace Yt
{
	class GuiFrame;
	class Renderer2D;
	class ResourceLoader;
	class Texture2D;
}

class Screen;

class Game
{
public:
	const std::unique_ptr<Screen> _logoScreen;
	const std::unique_ptr<Screen> _mainMenuScreen;
	const std::unique_ptr<Screen> _playMenuScreen;
	const std::unique_ptr<Screen> _gameScreen;
	const std::unique_ptr<Screen> _pauseScreen;
	const std::unique_ptr<Screen> _gameOverScreen;
	const std::unique_ptr<Screen> _topScoresScreen;
	const std::unique_ptr<Screen> _helpScreen;

	GameLogic _logic;
	int _startLevel = 0;
	GameGraphics _graphics;
	std::vector<std::pair<int, std::string>> _topScores;

	Game(Yt::ResourceLoader&);
	~Game() noexcept;

	void drawBackground(Yt::Renderer2D&) const;
	void drawGraphics(Yt::GuiFrame&) const;
	void drawShade(Yt::GuiFrame&) const;
	void setNextScreen(const std::unique_ptr<Screen>&);

	bool present(Yt::GuiFrame&);

private:
	Screen* _currentScreen = nullptr;
	Screen* _nextScreen = _logoScreen.get();
	const std::shared_ptr<const Yt::Texture2D> _backgroundTexture;
	const std::shared_ptr<const Yt::Texture2D> _cursorTexture;
};
