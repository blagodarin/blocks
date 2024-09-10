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
#include "textures.hpp"

#include <seir_data/blob.hpp>
#include <seir_data/storage.hpp>
#include <seir_graphics/color.hpp>
#include <seir_graphics/rectf.hpp>
#include <seir_gui/frame.hpp>
#include <seir_gui/layout.hpp>
#include <seir_renderer/2d.hpp>
#include <seir_renderer/renderer.hpp>

#include <cassert>

namespace
{
	struct AudioCallbacks : public seir::AudioCallbacks
	{
		void onPlaybackError(seir::AudioError) override {}
		void onPlaybackError(std::string&&) override {}
		void onPlaybackStarted() override {}
		void onPlaybackStopped() override {}
	};

	AudioCallbacks kAudioCallbacks;

	const seir::AudioFormat kAudioFormat{ seir::AudioSampleType::f32, seir::AudioChannelLayout::Stereo, seir::AudioFormat::kMaxSamplingRate };
	const seir::AudioDecoderPreferences kMusicPreferences{ .format = kAudioFormat, .loop = true };
	const seir::AudioDecoderPreferences kSoundPreferences{ .format = kAudioFormat, .loop = false };

	seir::RectF scaleToFill(const seir::SizeF& textureSize, const seir::SizeF& rectSize)
	{
		const auto widthRatio = textureSize._width / rectSize._width;
		const auto heightRatio = textureSize._height / rectSize._height;
		if (widthRatio < heightRatio)
		{
			const auto offsetY = (textureSize._height - rectSize._height * widthRatio) / 2;
			return seir::RectF{ { 0, offsetY }, seir::Vec2{ textureSize._width, textureSize._height - offsetY } };
		}
		const auto offsetX = (textureSize._width - rectSize._width * heightRatio) / 2;
		return seir::RectF{ { offsetX, 0 }, seir::Vec2{ textureSize._width - offsetX, textureSize._height } };
	}
}

Game::Game(seir::Storage& storage, seir::Renderer& renderer)
	: _logoScreen{ std::make_unique<LogoScreen>(*this) }
	, _mainMenuScreen{ std::make_unique<MainMenuScreen>(*this) }
	, _playMenuScreen{ std::make_unique<PlayMenuScreen>(*this) }
	, _gameScreen{ std::make_unique<GameScreen>(*this) }
	, _pauseScreen{ std::make_unique<PauseScreen>(*this) }
	, _gameOverScreen{ std::make_unique<GameOverScreen>(*this) }
	, _topScoresScreen{ std::make_unique<TopScoresScreen>(*this) }
	, _helpScreen{ std::make_unique<HelpScreen>(*this) }
	, _graphics{ renderer }
	, _audio{ seir::AudioPlayer::create(kAudioCallbacks) }
	, _menuMusic{ seir::AudioDecoder::create(storage.open("data/music/prelude_in_g_minor.aulos"), kMusicPreferences) }
	, _easyGameMusic{ seir::AudioDecoder::create(storage.open("data/music/grande_valse_brillante.aulos"), kMusicPreferences) }
	, _normalGameMusic{ seir::AudioDecoder::create(storage.open("data/music/hungarian_dance.aulos"), kMusicPreferences) }
	, _hardGameMusic{ seir::AudioDecoder::create(storage.open("data/music/turkish_march.aulos"), kMusicPreferences) }
	, _gameOverMusic{ seir::AudioDecoder::create(storage.open("data/music/fur_elise.aulos"), kMusicPreferences) }
	, _cancelSound{ seir::AudioDecoder::create(storage.open("data/sounds/cancel.aulos"), kSoundPreferences) }
	, _okSound{ seir::AudioDecoder::create(storage.open("data/sounds/ok.aulos"), kSoundPreferences) }
	, _backgroundTexture{ renderer.createTexture2D(::makeBackgroundTexture()) }
	, _cursorTexture{ renderer.createTexture2D(::makeCursorTexture(64)) }
{
	_topScores.emplace_back(250'000, "Grandm\xc3\xa6ster");
	_topScores.emplace_back(200'000, "Master");
	_topScores.emplace_back(150'000, "Professional");
	_topScores.emplace_back(100'000, "Amateur");
	_topScores.emplace_back(50'000, "Novice");
}

Game::~Game() = default;

void Game::drawBackground(seir::GuiFrame& gui) const
{
	gui.renderer().setTexture(_backgroundTexture);
	gui.renderer().setTextureRect(::scaleToFill(seir::SizeF{ _backgroundTexture->size() }, gui.size()));
	gui.renderer().setColor(seir::Rgba32::white());
	gui.renderer().addRect(seir::RectF{ gui.size() });
}

void Game::drawGraphics(seir::GuiFrame& gui) const
{
	seir::GuiLayout layout{ gui, seir::GuiLayout::Center{ 30, 26 } };
	gui.selectWhiteTexture();
	gui.renderer().setColor(seir::Rgba32::black(0x88));
	gui.renderer().addRect(layout.map({ { 1, 2 }, seir::SizeF{ 6, 5 } }));
	gui.renderer().addRect(layout.map({ { 10, 2 }, seir::SizeF{ 10, 22 } }));
	gui.renderer().addRect(layout.map({ { 23, 2 }, seir::SizeF{ 6, 2 } }));
	gui.renderer().addRect(layout.map({ { 23, 6 }, seir::SizeF{ 6, 2 } }));
	gui.renderer().addRect(layout.map({ { 23, 10 }, seir::SizeF{ 6, 2 } }));
	layout.setItemSize({ 5, 1 });
	layout.fromPoint({ 1.5, 2 }, { 1, 1 }, seir::GuiLayout::Axis::X);
	gui.addLabel("Next:");
	layout.fromPoint({ 23.5, 2 }, { 1, 1 }, seir::GuiLayout::Axis::Y);
	layout.setItemSpacing(3);
	gui.addLabel("Level:");
	gui.addLabel("Lines:");
	gui.addLabel("Score:");
	layout.fromPoint({ 23.5, 3 }, { 1, 1 }, seir::GuiLayout::Axis::Y);
	gui.addLabel(std::to_string(_logic.level()), seir::GuiAlignment::Right);
	gui.addLabel(std::to_string(_logic.lines()), seir::GuiAlignment::Right);
	gui.addLabel(std::to_string(_logic.score()), seir::GuiAlignment::Right);
	_graphics.drawField(gui.renderer(), layout.map({ { 9, 1 }, seir::SizeF{ 12, 24 } }), _logic.field(), _logic.current_figure());
	_graphics.drawNextFigure(gui.renderer(), layout.map({ { 2, 4 }, seir::SizeF{ 4, 2 } }), _logic.next_figure());
}

void Game::drawShade(seir::GuiFrame& gui) const
{
	gui.selectWhiteTexture();
	gui.renderer().setColor(seir::Rgba32::black(0x88));
	gui.renderer().addRect(seir::RectF{ gui.size() });
}

void Game::setNextScreen(const std::unique_ptr<Screen>& screen)
{
	_nextScreen = screen.get();
}

bool Game::present(seir::GuiFrame& gui)
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
		gui.renderer().setColor(seir::Rgba32::white());
		gui.renderer().addRect({ *cursor, seir::SizeF{ _cursorTexture->size() } });
	}
	return _nextScreen;
}
