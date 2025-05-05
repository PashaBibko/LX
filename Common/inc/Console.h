namespace LX
{
	// Enum of colors with their Win32 equivalent value //
	enum class Color
	{
		BLACK =			0,
		BLUE =			1,
		GREEN =			2,
		AQUA =			3,
		RED =			4,
		PURPLE =		5,
		YELLOW =		6,
		LIGHT_GRAY =	7,
		LIGHT_BLUE =	9,
		LIGHT_GREEN =	10,
		LIGHT_AQUA =	11,
		LIGHT_RED =		12,
		LIGHT_PURPLE =	13,
		LIGHT_YELLOW =	14,
		WHITE =			15
	};

	// Prints a string to std::cout with a certain color using Win32 API //
	extern "C" void COMMON_API PrintStringAsColor(const std::string& str, Color c);

	// Util function for getting a line of the source at a given index (used for errors) //
	inline std::string GetLineAtIndexOf(const std::string& src, const std::streamsize index) // <- Has to be inline because of C++ types
	{
		// Finds the start of the line //
		size_t start = src.rfind('\n', index);
		if (start == std::string::npos) { start = 0; } // None means first line
		else { start = start + 1; } // Skips new line char

		// Finds the end of the line //
		size_t end = src.find('\n', index);
		if (end == std::string::npos) { end = src.size(); } // None means last line

		// Returns the string between start and end //
		return src.substr(start, end - start);
	}
}
