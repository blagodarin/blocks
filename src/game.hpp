// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "graphics.hpp"
#include "logic.hpp"

#include <yttrium/audio/manager.h>
#include <yttrium/audio/sound.h>

#include <chrono>
#include <memory>
#include <string>
#include <vector>

namespace Yt
{
	class GuiFrame;
	class RenderManager;
	class Renderer2D;
	class Storage;
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

	const std::shared_ptr<Yt::AudioManager> _audio;
	const std::shared_ptr<Yt::AudioReader> _menuMusic;
	const std::shared_ptr<Yt::AudioReader> _easyGameMusic;
	const std::shared_ptr<Yt::AudioReader> _normalGameMusic;
	const std::shared_ptr<Yt::AudioReader> _hardGameMusic;
	const std::shared_ptr<Yt::AudioReader> _gameOverMusic;
	const std::shared_ptr<Yt::Sound> _cancelSound;
	const std::shared_ptr<Yt::Sound> _okSound;

	GameLogic _logic;
	int _startLevel = 0;
	GameGraphics _graphics;
	std::vector<std::pair<int, std::string>> _topScores;

	Game(Yt::Storage&, Yt::RenderManager&);
	~Game() noexcept;

	void drawBackground(Yt::Renderer2D&) const;
	void drawGraphics(Yt::GuiFrame&) const;
	void drawShade(Yt::GuiFrame&) const;
	void setNextScreen(const std::unique_ptr<Screen>&);

	bool present(Yt::GuiFrame&);

private:
	const std::shared_ptr<const Yt::Texture2D> _backgroundTexture;
	const std::shared_ptr<const Yt::Texture2D> _cursorTexture;
	Screen* _currentScreen = nullptr;
	Screen* _nextScreen = _logoScreen.get();
};
