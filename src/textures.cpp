// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#include "textures.hpp"

#include <yttrium/image/color.h>

Yt::Image makeBackgroundTexture()
{
	return Yt::Image::generateBgra32(1024, 1024, [](size_t x, size_t y) {
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
		return Yt::Bgra32{ 21 * i, 25 * i, 31 * i };
	});
}

Yt::Image makeCursorTexture(size_t size)
{
	return Yt::Image::generateBgra32(size, size, [size](size_t x, size_t y) {
		if (y > 2 * x || 2 * y < x || (y > 2 * (size - x) && x > 2 * (size - y)))
			return Yt::Bgra32{ 0, 0, 0, 0 };
		else
			return Yt::Bgra32{
				y * 0xff / (size - 1),
				x * 0xff / (size - 1),
				(size * size - x * y) * 0xff / (size * size),
			};
	});
}
