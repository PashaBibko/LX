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
		LX::PrintStringAsColor("Error: ", LX::Color::LIGHT_RED);
		std::cout << "Invalid character found in ";
		LX::PrintStringAsColor(file, LX::Color::WHITE);
		std::cout << " {";
		LX::PrintStringAsColor(std::string(1, invalid), LX::Color::LIGHT_RED);
		std::cout << "}:\n";
		std::cout << "Line: " << std::setw(lineNumberWidthInConsole) << line << " | " << lineContents << "\n";
		std::cout << "      " << std::setw(lineNumberWidthInConsole) << "" << " | " << std::setw(col - 1) << "";
		LX::PrintStringAsColor("^", LX::Color::LIGHT_RED);
		std::cout << "\n";
	}

	const char* InvalidCharInSource::ErrorType() const
	{
		return "Invalid char in source";
	}
}
