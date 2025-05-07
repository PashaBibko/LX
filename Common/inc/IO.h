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

	inline std::string ReadFileToString(const std::filesystem::path& path, const std::string errorName = "input file path")
	{
		// Verifies the file path is valid //
		ThrowIf<LX::InvalidFilePath>(std::filesystem::exists(path) == false, errorName, path);

		// Opens the file //
		std::ifstream file(path, std::ios::binary | std::ios::ate); // Opens in binary and at the end (microptimsation)
		ThrowIf<LX::InvalidFilePath>(file.is_open() == false, errorName, path);

		// Stores the length of the string and goes back to the beginning //
		const std::streamsize len = file.tellg(); // tellg returns length because it was opened at the end
		file.seekg(0, std::ios::beg);

		// Transfers the file contents to the output //
		std::string contents(len, '\0'); // Allocates an empty string which is the size of the file
		file.read(&contents[0], len);
		return contents;
	}

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

	// Util function for turning a a char to a string. Used to stop '\t' being printed as a tab //
	inline std::string CharAsStrLit(const char c)
	{
		switch (c)
		{
			// Stores them as pure string literals //
			case '\n': return R"(\n)";
			case '\t': return R"(\t)";
			case '\r': return R"(\r)";

			// Else returns a string of length one with the char inside //
			default: return std::string(1, c);
		}
	}
}
