// This file is part of the Blocks game.
// Copyright (C) Sergei Blagodarin.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <string_view>

namespace Yt
{
	class Storage;
}

void makeBackgroundTexture(Yt::Storage&, std::string_view name);
void makeCursorTexture(Yt::Storage&, std::string_view name, size_t size);
