// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "textures.hpp"

#include <seir_graphics/color.hpp>

namespace
{
	template <typename Callback>
	static seir::Image generateBgra32(uint32_t width, uint32_t height, Callback&& callback)
	{
		const seir::ImageInfo info{ width, height, seir::PixelFormat::Bgra32 };
		seir::Buffer buffer{ info.stride() * height };
		auto data = reinterpret_cast<uint8_t*>(buffer.data());
		for (uint32_t y = 0; y < height; ++y)
		{
			for (uint32_t x = 0; x < width; ++x)
			{
				const auto pixel = data + y * info.stride() + x * 4;
				const auto color = callback(x, y);
				pixel[0] = color._b;
				pixel[1] = color._g;
				pixel[2] = color._r;
				pixel[3] = color._a;
			}
		}
		return { info, std::move(buffer) };
	}
}

seir::Image makeBackgroundTexture()
{
	return ::generateBgra32(1024, 1024, [](uint32_t x, uint32_t y) {
		int i = 0;
		if ((x ^ y) % 3)
			++i;
		if ((x ^ y) % 5)
			++i;
		if ((x ^ y) % 7)
			++i;
		if ((x ^ y) % 11)
			++i;
		if (!((x ^ y) % 13))
			++i;
		if (!((x ^ y) % 17))
			++i;
		if (!((x ^ y) % 19))
			++i;
		if (!((x ^ y) % 23))
			++i;
		return seir::Rgba32{ 31 * i, 25 * i, 21 * i };
	});
}

seir::Image makeCursorTexture(uint32_t size)
{
	return ::generateBgra32(size, size, [size](uint32_t x, uint32_t y) {
		if (y > 2 * x || 2 * y < x || (y > 2 * (size - x) && x > 2 * (size - y)))
			return seir::Rgba32{ 0, 0, 0, 0 };
		else
			return seir::Rgba32{
				(size * size - x * y) * 0xff / (size * size),
				x * 0xff / (size - 1),
				y * 0xff / (size - 1),
			};
	});
}
