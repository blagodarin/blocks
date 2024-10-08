// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "logic.hpp"

#include <algorithm>
#include <cassert>

namespace
{
	// Game timing parameters.
	enum
	{
		MillisecondsPerFrame = 25, // 25 ms frames (40 frames per second).

		// Delays.

		StartupDelay = 20, // 0.5 s startup delay.
		FixDelay = 16,     // 0.4 s delay before the figure fixation.
		DelayFrames = 4,   // 0.1 s delay between the fixation and the next figure.

		// Speed values.

		BaseSpeed = 1,          // 1 point per frame (1 cell per second).
		AccelerationSpeed = 80, // 80 points (2 cells) per frame (80 cells per second).

		// Horizontal movement timing.

		HMoveDelay = 6,    // 150 ms initial delay.
		HMoveInterval = 2, // 50 ms successive delays.
	};

	enum
	{
		MaxFigureWidth = 4,
	};
}

GameLogic::Figure::Figure(Type type)
	: _type(type)
{
	switch (_type)
	{
	case I:
		// ....
		// XXXX
		_blocks[0] = { 0, 0 };
		_blocks[1] = { 1, 0 };
		_blocks[2] = { 2, 0 };
		_blocks[3] = { 3, 0 };
		_top_left = { 0, 2 };
		_bottom_right = { 3, -1 };
		break;

	case J:
		// .X..
		// .XXX
		_blocks[0] = { 1, 0 };
		_blocks[1] = { 2, 0 };
		_blocks[2] = { 3, 0 };
		_blocks[3] = { 1, 1 };
		_top_left = { 1, 1 };
		_bottom_right = { 3, -1 };
		break;

	case L:
		// ..X.
		// XXX.
		_blocks[0] = { 0, 0 };
		_blocks[1] = { 1, 0 };
		_blocks[2] = { 2, 0 };
		_blocks[3] = { 2, 1 };
		_top_left = { 0, 1 };
		_bottom_right = { 2, -1 };
		break;

	case O:
		// .XX.
		// .XX.
		_blocks[0] = { 1, 0 };
		_blocks[1] = { 2, 0 };
		_blocks[2] = { 1, 1 };
		_blocks[3] = { 2, 1 };
		_top_left = { 1, 1 };
		_bottom_right = { 2, 0 };
		break;

	case S:
		// ..XX
		// .XX.
		_blocks[0] = { 1, 0 };
		_blocks[1] = { 2, 0 };
		_blocks[2] = { 2, 1 };
		_blocks[3] = { 3, 1 };
		_top_left = { 1, 2 };
		_bottom_right = { 3, 0 };
		break;

	case T:
		// .X..
		// XXX.
		_blocks[0] = { 0, 0 };
		_blocks[1] = { 1, 0 };
		_blocks[2] = { 2, 0 };
		_blocks[3] = { 1, 1 };
		_top_left = { 0, 1 };
		_bottom_right = { 2, -1 };
		break;

	case Z:
		// XX..
		// .XX.
		_blocks[0] = { 1, 0 };
		_blocks[1] = { 2, 0 };
		_blocks[2] = { 0, 1 };
		_blocks[3] = { 1, 1 };
		_top_left = { 0, 2 };
		_bottom_right = { 2, 0 };
		break;

	default:
		// ....
		// ....
		break;
	}
}

std::array<GameLogic::Point, 4> GameLogic::Figure::blocks() const
{
	std::array<Point, 4> result;
	for (const auto& block : _blocks)
		result[static_cast<size_t>(&block - _blocks.data())] = { _offset.x + block.x, _offset.y + block.y * PointsPerRow };
	return result;
}

void GameLogic::Figure::turn_left()
{
	std::transform(_blocks.begin(), _blocks.end(), _blocks.begin(), [this](const Point& block) {
		return Point{ _top_left.x + (_top_left.y - block.y), _top_left.y - (_bottom_right.x - block.x) };
	});
}

void GameLogic::Figure::turn_right()
{
	std::transform(_blocks.begin(), _blocks.end(), _blocks.begin(), [this](const Point& block) {
		return Point{ _bottom_right.x - (_top_left.y - block.y), _top_left.y + (_top_left.x - block.x) };
	});
}

GameLogic::Field::Field()
{
	for (int y = 0; y < Height + HeightPadding; ++y)
		for (int x = 0; x < Width; ++x)
			blocks[y][x] = Figure::None;
}

int GameLogic::Field::collapse_full_rows()
{
	int result = 0;
	for (int row = Field::Height - 1; row >= 0; --row)
	{
		int cells_filled = 0;
		for (int column = 0; column < Field::Width; ++column)
			cells_filled += (blocks[row][column] != Figure::None);
		if (cells_filled == Width)
		{
			for (int y = row; y < Field::Height; ++y)
				for (int x = 0; x < Field::Width; ++x)
					blocks[y][x] = blocks[y + 1][x];
			++result;
		}
	}
	return result;
}

bool GameLogic::Field::is_overflown() const
{
	for (int row = Height; row < Height + HeightPadding; ++row)
		for (int column = 0; column < Width; ++column)
			if (blocks[row][column] != Figure::None)
				return true;
	return false;
}

bool GameLogic::Field::fit(Figure& figure) const
{
	const auto row_offset = figure.row_offset();
	bool shift = false;
	for (const auto& block : figure.blocks())
	{
		if (block.x < 0 || block.x >= Width || block.y < 0)
			return false;
		const auto row = block.y / PointsPerRow;
		if (row < Height && blocks[row][block.x] != Figure::None)
			return false;
		if (row + 1 < Height && blocks[row + 1][block.x] != Figure::None)
		{
			if (row_offset >= PointsPerRow / 4)
				return false;
			shift = true;
		}
	}
	if (shift)
		figure.move_down(row_offset);
	return true;
}

void GameLogic::Field::put_figure(const Figure& figure)
{
	for (const auto& block : figure.blocks())
		blocks[block.y / PointsPerRow][block.x] = figure.type();
}

bool GameLogic::advance(int milliseconds)
{
	if (!is_active())
		return false;

	_time_remainder += milliseconds;

	int frames = _time_remainder / MillisecondsPerFrame;
	_time_remainder %= MillisecondsPerFrame;

	for (bool horizontal = true; frames;)
	{
		switch (_state)
		{
		case Stopped:
			assert(false);
			break;

		case Waiting:
			process_falldown_delay(&frames);
			break;

		case Falling:
		case Fixing:
			if (horizontal)
			{
				for (auto offset = process_horizontal_movement(frames); offset;)
				{
					auto figure = _current_figure;
					if (offset > 0)
					{
						figure.move_right();
						--offset;
					}
					else
					{
						figure.move_left();
						++offset;
					}
					if (!_field.fit(figure))
						break;
					_current_figure = figure;
				}
			}
			process_falldown(&frames);
			if (_state == Fixing && frames)
				process_fixation(&frames);
			break;

		case Finished:
			frames = 0;
			break;
		}
		horizontal = false;
	}
	return true;
}

GameLogic::Figure GameLogic::current_figure() const
{
	return (_state == Falling || _state == Fixing) && !_paused ? _current_figure : Figure();
}

GameLogic::Field GameLogic::field() const
{
	return _state != Stopped && !_paused ? _field : Field{};
}

bool GameLogic::is_active() const
{
	return _state != Stopped && _state != Finished && !_paused;
}

GameLogic::Figure GameLogic::next_figure() const
{
	return _state != Stopped && !_paused ? _next_figure : Figure{};
}

void GameLogic::pause()
{
	if (_state != Stopped && _state != Finished)
		_paused = true;
}

void GameLogic::resume()
{
	if (_state != Stopped && _state != Finished)
		_paused = false;
}

void GameLogic::set_acceleration(bool accelerate)
{
	if (is_active())
		_is_accelerating = accelerate;
}

void GameLogic::set_left_movement(bool move)
{
	if (!is_active() || _is_moving_left == move)
		return;
	_is_moving_left = move;
	if (_is_moving_left)
	{
		if (_state != Waiting)
		{
			auto figure = _current_figure;
			figure.move_left();
			if (_field.fit(figure))
				_current_figure = figure;
		}
		_left_movement_timer = HMoveInterval - HMoveDelay;
	}
}

void GameLogic::set_right_movement(bool move)
{
	if (!is_active() || _is_moving_right == move)
		return;
	_is_moving_right = move;
	if (_is_moving_right)
	{
		if (_state != Waiting)
		{
			auto figure = _current_figure;
			figure.move_right();
			if (_field.fit(figure))
				_current_figure = figure;
		}
		_right_movement_timer = HMoveInterval - HMoveDelay;
	}
}

void GameLogic::start(int start_level)
{
	_field = {};
	_next_figure = Figure(static_cast<Figure::Type>(_random_distribution(_random_engine)));
	_time_remainder = 0;

	_is_accelerating = false;
	_acceleration_bonus = 0;

	_is_moving_left = false;
	_left_movement_timer = 0;

	_is_moving_right = false;
	_right_movement_timer = 0;

	_score = 0;
	_lines = 0;
	_level = start_level;
	_speed = BaseSpeed * _level;

	_state = Waiting;
	_timeout = StartupDelay;

	_paused = false;
}

bool GameLogic::turn_left()
{
	if (!is_active() || _state == Waiting)
		return false;
	auto figure = _current_figure;
	figure.turn_left();
	if (!_field.fit(figure))
		return false;
	_current_figure = figure;
	return true;
}

bool GameLogic::turn_right()
{
	if (!is_active() || _state == Waiting)
		return false;
	auto figure = _current_figure;
	figure.turn_right();
	if (!_field.fit(figure))
		return false;
	_current_figure = figure;
	return true;
}

void GameLogic::process_falldown(int* frames)
{
	const auto speed = (_is_accelerating && _speed < AccelerationSpeed) ? AccelerationSpeed : _speed;
	const auto expected_distance = *frames * speed;

	auto actual_distance = expected_distance;
	for (const auto& block : _current_figure.blocks())
	{
		actual_distance = std::min(actual_distance, block.y);
		for (auto row = std::min(block.y / PointsPerRow, int{ Field::Height }); row > 0; --row)
		{
			if (_field.blocks[row - 1][block.x] != Figure::None)
			{
				actual_distance = std::min(actual_distance, block.y - row * PointsPerRow);
				break;
			}
		}
	}
	_current_figure.move_down(actual_distance);

	if (actual_distance)
	{
		_fixation_delay = 0;
		if (_is_accelerating)
			_acceleration_bonus += actual_distance;
	}

	// The division throws away the entire contact frame, but it can't be helped.

	int remaining_frames = (expected_distance - actual_distance) / speed;
	*frames = remaining_frames;

	if (remaining_frames && _state != Fixing)
	{
		_state = Fixing;
		_timeout = FixDelay;
	}
}

void GameLogic::process_falldown_delay(int* frames)
{
	int remaining_frames = *frames;
	if (_timeout > remaining_frames)
	{
		_timeout -= remaining_frames;
		remaining_frames = 0;
	}
	else
	{
		remaining_frames -= _timeout;
		_timeout = 0;
	}

	int delay_frames = *frames - remaining_frames;
	if (delay_frames)
		process_horizontal_movement(delay_frames); // Update the timers without moving the figure.
	*frames = remaining_frames;

	if (!_timeout)
	{
		_state = Falling;
		_current_figure = _next_figure;
		_current_figure.put_at({ (Field::Width - MaxFigureWidth) / 2, Field::Height * PointsPerRow });
		_next_figure = Figure(static_cast<Figure::Type>(_random_distribution(_random_engine)));
	}
}

void GameLogic::process_fixation(int* frames)
{
	int remaining_frames = *frames;
	if (_timeout > remaining_frames)
	{
		_timeout -= remaining_frames;
		remaining_frames = 0;
	}
	else
	{
		remaining_frames -= _timeout;
		_timeout = 0;
	}
	*frames = remaining_frames;

	if (!_timeout)
	{
		_field.put_figure(_current_figure);
		update_score();
		if (_field.is_overflown())
		{
			_state = Finished;
			_paused = false;
		}
		else
		{
			_state = Waiting;
			_timeout = DelayFrames;
			_speed = BaseSpeed * _level;
		}
	}
}

void GameLogic::update_score()
{
	const auto collapsed = _field.collapse_full_rows();
	_score += (collapsed * (collapsed + 1) * 50 + _acceleration_bonus / PointsPerRow) * _level;
	_lines += collapsed;
	_level += _lines / Field::Width - (_lines - collapsed) / Field::Width;
	_acceleration_bonus = 0;
}

int GameLogic::process_horizontal_movement(int frames)
{
	int offset = 0;

	if (_is_moving_left)
	{
		_left_movement_timer += frames;
		while (_left_movement_timer >= HMoveInterval)
		{
			_left_movement_timer -= HMoveInterval;
			--offset;
		}
	}

	if (_is_moving_right)
	{
		_right_movement_timer += frames;
		while (_right_movement_timer >= HMoveInterval)
		{
			_right_movement_timer -= HMoveInterval;
			++offset;
		}
	}

	return offset;
}
