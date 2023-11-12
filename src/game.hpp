// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "graphics.hpp"
#include "logic.hpp"

#include <seir_audio/decoder.hpp>
#include <seir_audio/player.hpp>

#include <chrono>
#include <memory>
#include <string>
#include <vector>

namespace Yt
{
	class GuiFrame;
	class RenderManager;
	class Renderer2D;
	class Texture2D;
}

namespace seir
{
	class Storage;
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

	const seir::UniquePtr<seir::AudioPlayer> _audio;
	const seir::SharedPtr<seir::AudioDecoder> _menuMusic;
	const seir::SharedPtr<seir::AudioDecoder> _easyGameMusic;
	const seir::SharedPtr<seir::AudioDecoder> _normalGameMusic;
	const seir::SharedPtr<seir::AudioDecoder> _hardGameMusic;
	const seir::SharedPtr<seir::AudioDecoder> _gameOverMusic;
	const seir::SharedPtr<seir::AudioDecoder> _cancelSound;
	const seir::SharedPtr<seir::AudioDecoder> _okSound;

	GameLogic _logic;
	int _startLevel = 0;
	GameGraphics _graphics;
	std::vector<std::pair<int, std::string>> _topScores;

	Game(seir::Storage&, Yt::RenderManager&);
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
