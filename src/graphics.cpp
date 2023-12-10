// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "graphics.hpp"

#include <yttrium/geometry/margins.h>
#include <yttrium/geometry/rect.h>
#include <yttrium/renderer/2d.h>
#include <yttrium/renderer/manager.h>
#include <yttrium/renderer/texture.h>

#include <seir_image/image.hpp>
#include <seir_math/vec.hpp>

namespace
{
	struct Rgb
	{
		uint8_t _r = 0;
		uint8_t _g = 0;
		uint8_t _b = 0;

		Rgb(uint8_t r, uint8_t g, uint8_t b) noexcept
			: _r{ r }, _g{ g }, _b{ b } {}
	};

	const std::array<int, 24> weights{
		2, 2, 2, // Gray.
		1, 0, 0, // Red.
		1, 2, 0, // Orange.
		1, 1, 0, // Yellow.
		0, 1, 0, // Green.
		0, 1, 1, // Light blue.
		0, 0, 1, // Blue.
		2, 0, 1, // Purple.
	};

	enum
	{
		TopLeft,
		Top,
		TopRight,
		Left,
		Center,
		Right,
		BottomLeft,
		Bottom,
		BottomRight
	};

	const std::array<int, 9> pattern{
		// clang-format off
		0, 0, 3,
		1, 2, 3,
		1, 4, 4,
		// clang-format on
	};

	constexpr auto FragmentSize = 32u;
	constexpr auto FragmentCount = 8u;
	constexpr auto Border = 3;

	Rgb pixelColor(size_t block, size_t x, size_t y)
	{
		const auto pixelType = [](auto x_, auto y_) {
			const auto rx = FragmentSize - 1 - x_;
			if (y_ < Border)
			{
				if (x_ < Border)
					return x_ > y_ ? Top : (x_ < y_ ? Left : TopLeft);
				else if (rx < Border)
					return rx > y_ ? Top : (rx < y_ ? Right : TopRight);
				else
					return Top;
			}
			const auto ry = FragmentSize - 1 - y_;
			if (ry < Border)
			{
				if (x_ < Border)
					return x_ > ry ? Bottom : (x_ < ry ? Left : BottomLeft);
				else if (rx < Border)
					return rx > ry ? Bottom : (rx < ry ? Right : BottomRight);
				else
					return Bottom;
			}
			return x_ < Border ? Left : (rx < Border ? Right : Center);
		};
		const auto offset = 20 * pattern[pixelType(x, y)];
		const auto scale = [block, offset](size_t index) {
			const auto weight = weights[block * 3 + index];
			return static_cast<uint8_t>(weight ? 255 / weight - offset : 0);
		};
		return Rgb{ scale(0), scale(1), scale(2) };
	}

	seir::Image makeBlocksImage()
	{
		const seir::ImageInfo info{ FragmentSize, FragmentSize * FragmentCount, seir::PixelFormat::Bgra32 };
		seir::Buffer buffer{ info.frameSize() };
		for (size_t i = 0; i < FragmentCount; ++i)
		{
			for (size_t y = 0; y < FragmentSize; ++y)
			{
				for (size_t x = 0; x < FragmentSize; ++x)
				{
					const auto pixel = reinterpret_cast<uint8_t*>(buffer.data()) + (i * FragmentSize + y) * info.stride() + x * 4;
					const auto color = ::pixelColor(i, x, y);
					pixel[0] = color._b;
					pixel[1] = color._g;
					pixel[2] = color._r;
					pixel[3] = 0xff;
				}
			}
		}
		return { info, std::move(buffer) };
	}

	Yt::RectF blockRect(int index)
	{
		return { { 1, static_cast<float>(index) * FragmentSize + 1.f }, Yt::SizeF{ FragmentSize - 2, FragmentSize - 2 } };
	}
}

GameGraphics::GameGraphics(Yt::RenderManager& manager)
	: _blocksTexture{ manager.create_texture_2d(::makeBlocksImage()) }
{
}

void GameGraphics::drawField(Yt::Renderer2D& renderer, const Yt::RectF& rect, const GameLogic::Field& field, const GameLogic::Figure& currentFigure) const
{
	static const int totalWidth = 1 + GameLogic::Field::Width + 1;
	static const int totalHeight = 1 + GameLogic::Field::Height + 1;
	const Yt::SizeF blockSize{ rect.width() / totalWidth, rect.height() / totalHeight };
	renderer.setTexture(_blocksTexture);
	drawFieldBlocks(renderer, rect, blockSize, field);
	drawFieldFigure(renderer, rect, blockSize, currentFigure);
	drawFieldFrame(renderer, rect, blockSize);
}

void GameGraphics::drawNextFigure(Yt::Renderer2D& renderer, const Yt::RectF& rect, const GameLogic::Figure& figure) const
{
	if (figure.type() == GameLogic::Figure::None)
		return;
	renderer.setTexture(_blocksTexture);
	setTextureRect(renderer, figure.type());
	const Yt::SizeF blockSize{ rect.width() / 4, rect.height() / 2 };
	for (const auto& block : figure.blocks())
		drawBlock(renderer, rect, blockSize, block.x, 1 - block.y / GameLogic::PointsPerRow);
}

void GameGraphics::drawBlock(Yt::Renderer2D& renderer, const Yt::RectF& rect, const Yt::SizeF& blockSize, float x, float y) const
{
	renderer.addRect({ { rect.left() + x * blockSize._width, rect.top() + y * blockSize._height }, blockSize });
}

void GameGraphics::drawFieldBlocks(Yt::Renderer2D& renderer, const Yt::RectF& rect, const Yt::SizeF& block_size, const GameLogic::Field& field) const
{
	for (int y = 0; y < GameLogic::Field::Height; ++y)
	{
		for (int x = 0; x < GameLogic::Field::Width; ++x)
		{
			const auto figureType = field.blocks[y][x];
			if (figureType == GameLogic::Figure::None)
				continue;
			setTextureRect(renderer, figureType);
			drawBlock(renderer, rect, block_size, 1 + x, GameLogic::Field::Height - y);
		}
	}
}

void GameGraphics::drawFieldFigure(Yt::Renderer2D& renderer, const Yt::RectF& rect, const Yt::SizeF& block_size, const GameLogic::Figure& figure) const
{
	static const seir::Vec2 frameOffset{ 1, GameLogic::Field::Height };
	if (figure.type() == GameLogic::Figure::None)
		return;
	setTextureRect(renderer, figure.type());
	for (const auto& block : figure.blocks())
		if (block.y < GameLogic::Field::Height * GameLogic::PointsPerRow)
			drawBlock(renderer, rect, block_size, frameOffset.x + static_cast<float>(block.x), frameOffset.y - static_cast<float>(block.y) / GameLogic::PointsPerRow);
}

void GameGraphics::drawFieldFrame(Yt::Renderer2D& renderer, const Yt::RectF& rect, const Yt::SizeF& blockSize) const
{
	static const int totalWidth = 1 + GameLogic::Field::Width + 1;
	static const int totalHeight = 1 + GameLogic::Field::Height + 1;
	setTextureRect(renderer, GameLogic::Figure::None);
	for (int i = 0; i < totalWidth; ++i)
		drawBlock(renderer, rect, blockSize, i, 0);
	for (int i = 1; i < totalHeight - 1; ++i)
	{
		drawBlock(renderer, rect, blockSize, 0, i);
		drawBlock(renderer, rect, blockSize, totalWidth - 1, i);
	}
	for (int i = 0; i < totalWidth; ++i)
		drawBlock(renderer, rect, blockSize, i, totalHeight - 1);
}

void GameGraphics::setTextureRect(Yt::Renderer2D& renderer, GameLogic::Figure::Type figureType) const
{
	const auto figureIndex = figureType == GameLogic::Figure::None ? 0 : static_cast<int>(figureType) + 1;
	renderer.setTextureRect(::blockRect(figureIndex));
}
