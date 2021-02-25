// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "graphics.hpp"

#include <yttrium/application.h>
#include <yttrium/audio/format.h>
#include <yttrium/audio/manager.h>
#include <yttrium/audio/utils.h>
#include <yttrium/exceptions.h>
#include <yttrium/gui/gui.h>
#include <yttrium/image/image.h>
#include <yttrium/image/utils.h>
#include <yttrium/logger.h>
#include <yttrium/main.h>
#include <yttrium/renderer/modifiers.h>
#include <yttrium/renderer/pass.h>
#include <yttrium/resource_loader.h>
#include <yttrium/script/args.h>
#include <yttrium/script/context.h>
#include <yttrium/script/value.h>
#include <yttrium/storage/source.h>
#include <yttrium/storage/storage.h>
#include <yttrium/storage/writer.h>
#include <yttrium/window.h>

#include <numbers>

namespace
{
	void makeButtonsTexture(Yt::Storage& storage, const std::string& name, size_t button_size)
	{
		constexpr size_t button_styles = 4;
		storage.attach_buffer(name, Yt::make_bgra32_tga(button_size, button_size * button_styles, [button_size](size_t, size_t y) {
			const auto style = y / button_size;
			return Yt::Bgra32{ 0xff, 0x44 * style, 0x44 * style };
		}));
	}

	void makeCursorTexture(Yt::Storage& storage, const std::string& name, size_t size)
	{
		storage.attach_buffer(name, Yt::make_bgra32_tga(size, size, [size](size_t x, size_t y) {
			if (y > 2 * x || 2 * y < x || (y > 2 * (size - x) && x > 2 * (size - y)))
				return Yt::Bgra32{ 0, 0, 0, 0 };
			else
				return Yt::Bgra32{
					y * 0xff / (size - 1),
					x * 0xff / (size - 1),
					(size * size - x * y) * 0xff / (size * size),
				};
		}));
	}

	void makeSound(Yt::Storage& storage, const std::string& name)
	{
		Yt::Buffer buffer;
		{
			constexpr size_t frequency = 44100;
			constexpr size_t duration = frequency / 4; // 0.25 s.

			Yt::Writer writer{ buffer };
			if (Yt::write_wav_header(writer, { Yt::AudioSample::i16, 1, frequency }, duration))
			{
				constexpr auto time_step = 440.0 / frequency;

				for (size_t i = 0; i < duration; ++i)
				{
					const auto base = std::numeric_limits<int16_t>::max() * std::sin(2 * std::numbers::pi_v<double> * time_step * static_cast<double>(i));
					const auto amplitude = static_cast<double>(duration - i) / duration;
					writer.write(static_cast<int16_t>(base * amplitude));
				}
			}
		}
		storage.attach_buffer(name, std::move(buffer));
	}

	template <typename T, typename... Args>
	auto tryCreate(Args&&... args) -> decltype(T::create(std::forward<Args>(args)...))
	{
		try
		{
			return T::create(std::forward<Args>(args)...);
		}
		catch (const std::runtime_error& e)
		{
			Yt::Logger::log(e.what());
			return {};
		}
	}

	class LogoCanvas : public Yt::Canvas
	{
	public:
		explicit LogoCanvas(Yt::Gui& gui)
			: _gui{ gui } {}

		void on_draw(Yt::RenderPass& pass, const Yt::RectF& rect, std::chrono::milliseconds screen_time) override
		{
			const auto ms = screen_time.count();
			if (ms >= 4000)
				_gui.notify("finished");
			const auto opacity = std::min(1.f, std::abs(static_cast<float>(ms - 500) / 1500.f - 1.f));
			Yt::PushTexture texture{ pass, nullptr };
			pass.draw_rect(rect, { 0.f, 0.f, 0.f, opacity });
		}

	private:
		Yt::Gui& _gui;
	};

	class FieldCanvas : public Yt::Canvas
	{
	public:
		FieldCanvas(const GameLogic& logic, const GameGraphics& graphics)
			: _logic{ logic }, _graphics{ graphics } {}

		void on_draw(Yt::RenderPass& pass, const Yt::RectF& rect, std::chrono::milliseconds) override
		{
			_graphics.drawField(pass, rect, _logic.field(), _logic.current_figure());
		}

	private:
		const GameLogic& _logic;
		const GameGraphics& _graphics;
	};

	class NextFigureCanvas : public Yt::Canvas
	{
	public:
		NextFigureCanvas(const GameLogic& logic, const GameGraphics& graphics)
			: _logic{ logic }, _graphics{ graphics } {}

		void on_draw(Yt::RenderPass& pass, const Yt::RectF& rect, std::chrono::milliseconds) override
		{
			_graphics.drawNextFigure(pass, rect, _logic.next_figure());
		}

	private:
		const GameLogic& _logic;
		const GameGraphics& _graphics;
	};

	class ScoreTable
	{
	public:
		explicit ScoreTable(Yt::ScriptContext& script)
			: _script{ script } {}

		void addScore(const std::string& name, int score)
		{
			const auto i = std::find_if(_scores.cbegin(), _scores.cend(), [score](const auto& table_score) { return table_score.second < score; });
			if (i != _scores.cend())
			{
				_scores.emplace(i, name, score);
				_scores.resize(_scores.size() - 1);
				updateVariables();
			}
		}

		void setScoreCount(int count)
		{
			_scores.resize(static_cast<size_t>(count));
			updateVariables();
		}

	private:
		void updateVariables() const
		{
			for (const auto& score : _scores)
			{
				const auto index = &score - _scores.data() + 1;
				_script.set("name" + std::to_string(index), score.first);
				_script.set("score" + std::to_string(index), score.second);
			}
		}

	private:
		Yt::ScriptContext& _script;
		std::vector<std::pair<std::string, int>> _scores;
	};
}

int ymain(int, char**)
{
	Yt::Logger logger;
	Yt::Application application;

	Yt::Window window{ application };

	Yt::ScriptContext script;

	GameLogic logic;
	script.define("game_pause", [&logic](const Yt::ScriptCall&) { logic.pause(); });
	script.define("game_start", [&logic](const Yt::ScriptCall& call) { logic.start(call._context.get_int("start_level", 1)); });
	script.define("game_stop", [&logic](const Yt::ScriptCall&) { logic.pause(); });
	script.define("game_resume", [&logic](const Yt::ScriptCall&) { logic.resume(); });
	script.define("move_down", 1, [&logic](const Yt::ScriptCall& call) { logic.set_acceleration(call._args[0]->to_int()); });
	script.define("move_left", 1, [&logic](const Yt::ScriptCall& call) { logic.set_left_movement(call._args[0]->to_int()); });
	script.define("move_right", 1, [&logic](const Yt::ScriptCall& call) { logic.set_right_movement(call._args[0]->to_int()); });
	script.define("screenshot", [&window](const Yt::ScriptCall&) { window.take_screenshot(); });
	script.define("turn_left", [&logic](const Yt::ScriptCall&) { logic.turn_left(); });
	script.define("turn_right", [&logic](const Yt::ScriptCall&) { logic.turn_right(); });

	ScoreTable scoreTable{ script };
	script.define("add_score", 2, [&scoreTable](const Yt::ScriptCall& call) { scoreTable.addScore(call._args[0]->string(), call._args[1]->to_int()); });
	script.define("set_score_count", 1, [&scoreTable](const Yt::ScriptCall& call) { scoreTable.setScoreCount(call._args[0]->to_int()); });

	Yt::Storage storage{ Yt::Storage::UseFileSystem::Never };
	storage.attach_package("blocks.ypq");
	::makeButtonsTexture(storage, "data/textures/buttons.tga", 16);
	::makeCursorTexture(storage, "data/textures/cursor.tga", 64);
	::makeSound(storage, "data/sounds/sound.wav");

	const auto audio = tryCreate<Yt::AudioManager>();

	Yt::ResourceLoader resourceLoader{ storage, &window.render_manager() };
	Yt::Gui gui{ "data/gui.ion", resourceLoader, script, audio };
	gui.on_quit([&window] { window.close(); });

	window.on_key_event([&gui](const Yt::KeyEvent& event) { gui.process_key_event(event); });
	window.on_render([&gui](Yt::RenderPass& pass, const Yt::Vector2& cursor, const Yt::RenderReport&) { gui.draw(pass, cursor); });
	window.on_screenshot([](Yt::Image&& image) { image.save_as_screenshot(Yt::ImageFormat::Jpeg, 90); });
	window.on_text_input([&gui](std::string_view text) { gui.process_text_input(text); });

	GameGraphics graphics{ window.render_manager() };

	LogoCanvas logo_canvas{ gui };
	gui.bind_canvas("logo", logo_canvas);

	FieldCanvas field_canvas{ logic, graphics };
	gui.bind_canvas("field", field_canvas);

	NextFigureCanvas next_figure_canvas{ logic, graphics };
	gui.bind_canvas("next", next_figure_canvas);

	gui.start();
	window.set_title(gui.title());
	if (const auto source = resourceLoader.open(gui.icon_path()))
		if (const auto image = Yt::Image::load(*source))
			window.set_icon(*image);
	window.show();
	application.run([&script, &gui, &logic](const std::chrono::milliseconds& advance) {
		if (logic.advance(static_cast<int>(advance.count())))
		{
			script.set("score", logic.score());
			script.set("lines", logic.lines());
			script.set("level", logic.level());
			if (logic.has_finished())
				gui.notify("game_over");
		}
	});
	return 0;
}
