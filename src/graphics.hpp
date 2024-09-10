// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <seir_base/shared_ptr.hpp>

#include "logic.hpp"

namespace seir
{
	class RectF;
	class Renderer;
	class Renderer2D;
	class SizeF;
	class Texture2D;
}

class GameGraphics
{
public:
	explicit GameGraphics(seir::Renderer&);

	void drawField(seir::Renderer2D&, const seir::RectF&, const GameLogic::Field&, const GameLogic::Figure&) const;
	void drawNextFigure(seir::Renderer2D&, const seir::RectF&, const GameLogic::Figure&) const;

private:
	void drawBlock(seir::Renderer2D&, const seir::RectF&, const seir::SizeF& blockSize, float x, float y) const;
	void drawBlock(seir::Renderer2D& context, const seir::RectF& rect, const seir::SizeF& blockSize, int x, int y) const { drawBlock(context, rect, blockSize, static_cast<float>(x), static_cast<float>(y)); }
	void drawFieldBlocks(seir::Renderer2D&, const seir::RectF&, const seir::SizeF& blockSize, const GameLogic::Field&) const;
	void drawFieldFigure(seir::Renderer2D&, const seir::RectF&, const seir::SizeF& blockSize, const GameLogic::Figure&) const;
	void drawFieldFrame(seir::Renderer2D&, const seir::RectF&, const seir::SizeF& blockSize) const;
	void setTextureRect(seir::Renderer2D&, GameLogic::Figure::Type) const;

private:
	const seir::SharedPtr<seir::Texture2D> _blocksTexture;
};
