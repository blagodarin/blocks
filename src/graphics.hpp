// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "logic.hpp"

#include <yttrium/renderer/texture.h>

#include <memory>

namespace Yt
{
	class RectF;
	class RenderManager;
	class RenderPass;
	class SizeF;
	class Texture2D;
}

class GameGraphics
{
public:
	explicit GameGraphics(Yt::RenderManager&);

	void drawField(Yt::RenderPass&, const Yt::RectF&, const GameLogic::Field&, const GameLogic::Figure&) const;
	void drawNextFigure(Yt::RenderPass&, const Yt::RectF&, const GameLogic::Figure&) const;

private:
	void drawBlock(Yt::RenderPass&, const Yt::RectF&, const Yt::SizeF& block_size, float x, float y) const;
	void drawBlock(Yt::RenderPass& context, const Yt::RectF& rect, const Yt::SizeF& block_size, int x, int y) const { drawBlock(context, rect, block_size, static_cast<float>(x), static_cast<float>(y)); }
	void drawFieldBlocks(Yt::RenderPass&, const Yt::RectF&, const Yt::SizeF& block_size, const GameLogic::Field&) const;
	void drawFieldFigure(Yt::RenderPass&, const Yt::RectF&, const Yt::SizeF& block_size, const GameLogic::Figure&) const;
	void drawFieldFrame(Yt::RenderPass&, const Yt::RectF&, const Yt::SizeF& block_size) const;
	void setTextureRect(Yt::RenderPass&, GameLogic::Figure::Type) const;

private:
	const std::unique_ptr<const Yt::Texture2D> _blocksTexture;
};
