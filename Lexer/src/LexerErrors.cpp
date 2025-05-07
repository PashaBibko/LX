#include <LX-Common.h>

#include <LexerErrors.h>

#include <LexerInfo.h>

namespace LX
{
	InvalidCharInSource::InvalidCharInSource(const LexerInfo& info, const std::string _file)
		: col(info.column), line(info.line), file(_file), lineContents(GetLineAtIndexOf(info.source, info.index)), invalid(info.source[info.index])
	{}

	void InvalidCharInSource::PrintToConsole() const
	{
		// Calculates the length of the line number in the console so it is formatted correctly //
		std::ostringstream oss;
		oss << std::setw(3) << line;
		size_t lineNumberWidthInConsole = std::max(oss.str().size(), (size_t)3);

		// Prints the error with the relevant information to the console //
		std::cout << "\n";
		PrintAsColor<Color::LIGHT_RED>("Error: ");
		std::cout << "Invalid character found in ";
		PrintAsColor<Color::WHITE>(file);
		std::cout << " {";
		PrintAsColor<Color::LIGHT_RED>(std::string(1, invalid));
		std::cout << "}:\n";
		std::cout << "Line: " << std::setw(lineNumberWidthInConsole) << line << " | " << lineContents << "\n";
		std::cout << "      " << std::setw(lineNumberWidthInConsole) << "" << " | " << std::setw(col - 1) << "";
		PrintAsColor<Color::LIGHT_RED>("^");
		std::cout << "\n";
	}

	const char* InvalidCharInSource::ErrorType() const
	{
		return "Invalid char in source";
	}
}
