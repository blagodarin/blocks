// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "logic.hpp"

#include <memory>

namespace seir
{
	class RectF;
	class SizeF;
}

namespace Yt
{
	class RenderManager;
	class Renderer2D;
	class Texture2D;
}

class GameGraphics
{
public:
	explicit GameGraphics(Yt::RenderManager&);

	void drawField(Yt::Renderer2D&, const seir::RectF&, const GameLogic::Field&, const GameLogic::Figure&) const;
	void drawNextFigure(Yt::Renderer2D&, const seir::RectF&, const GameLogic::Figure&) const;

private:
	void drawBlock(Yt::Renderer2D&, const seir::RectF&, const seir::SizeF& block_size, float x, float y) const;
	void drawBlock(Yt::Renderer2D& context, const seir::RectF& rect, const seir::SizeF& block_size, int x, int y) const { drawBlock(context, rect, block_size, static_cast<float>(x), static_cast<float>(y)); }
	void drawFieldBlocks(Yt::Renderer2D&, const seir::RectF&, const seir::SizeF& block_size, const GameLogic::Field&) const;
	void drawFieldFigure(Yt::Renderer2D&, const seir::RectF&, const seir::SizeF& block_size, const GameLogic::Figure&) const;
	void drawFieldFrame(Yt::Renderer2D&, const seir::RectF&, const seir::SizeF& block_size) const;
	void setTextureRect(Yt::Renderer2D&, GameLogic::Figure::Type) const;

private:
	const std::shared_ptr<const Yt::Texture2D> _blocksTexture;
};
