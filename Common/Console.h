#pragma once

#include <string>

namespace LX
{
	enum class Color
	{
		BLACK = 0,
		BLUE = 1,
		GREEN = 2,
		AQUA = 3,
		RED = 4,
		PURPLE = 5,
		YELLOW = 6,
		LIGHT_GRAY = 7,
		LIGHT_BLUE = 9,
		LIGHT_GREEN = 10,
		LIGHT_AQUA = 11,
		LIGHT_RED = 12,
		LIGHT_PURPLE = 13,
		LIGHT_YELLOW = 14,
		WHITE = 15
	};

	void PrintStringAsColor(const std::string& str, Color c);
}
