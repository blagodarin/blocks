// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#pragma once

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

	std::vector<std::pair<int, std::string>> _topScores;

	Game(Yt::ResourceLoader&);
	~Game() noexcept;

	void drawBackground(Yt::Renderer2D&) const;
	bool present(Yt::GuiFrame&, const std::chrono::steady_clock::duration& duration);

private:
	Screen* _currentScreen = _logoScreen.get();
	const std::shared_ptr<const Yt::Texture2D> _backgroundTexture;
};
