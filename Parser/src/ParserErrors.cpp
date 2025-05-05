#include <LX-Common.h>

#include <Parser.h>

namespace LX
{
	void IRGenerationError::PrintToConsole() const
	{
	}

	const char* IRGenerationError::ErrorType() const
	{
		return "IR Generation Error";
	}

	UnexpectedToken::UnexpectedToken(Token::TokenType _expected, std::string _override, Token _got)
		: expected(_expected), override(_override), got(_got)
	{}

	void UnexpectedToken::PrintToConsole() const
	{
		// Calculates the length of the line number in the console so it is formatted correctly //
		std::ostringstream oss;
		oss << std::setw(3) << got.line;
		size_t lineNumberWidthInConsole = std::max(oss.str().size(), (size_t)3);

		// Gets the line of the error //
		std::string line = LX::GetLineAtIndexOf(*s_Source, got.index);

		// Prints the error to the console with the relevant info //
		std::cout << "\n";
		LX::PrintStringAsColor("Error: ", LX::Color::LIGHT_RED);
		std::cout << "Incorrect syntax in ";
		LX::PrintStringAsColor(s_SourceFile->filename().string(), LX::Color::WHITE);
		std::cout << ", found ";
		LX::PrintStringAsColor(LX::ToString(got.type).c_str(), LX::Color::WHITE);
		std::cout << " expected: ";

		// Allows the error to have a custom type that is printed to the console //
		if (expected == LX::Token::UNDEFINED) { LX::PrintStringAsColor(override.c_str(), LX::Color::WHITE); }
		else { LX::PrintStringAsColor(LX::ToString(expected).c_str(), LX::Color::WHITE); }
		std::cout << "\n";

		// Prints the code with the error to the console //
		std::string errorSquiggle(got.length, '~');
		std::cout << "Line: " << std::setw(lineNumberWidthInConsole) << got.line << " | " << line << "\n";
		std::cout << "      " << std::setw(lineNumberWidthInConsole) << "" << " | " << std::setw(got.column) << "";
		LX::PrintStringAsColor(errorSquiggle.c_str(), LX::Color::LIGHT_RED);
		std::cout << "\n";
	}

	const char* UnexpectedToken::ErrorType() const
	{
		return "Unexpected Token";
	}

	void Scope::VariableAlreadyExists::PrintToConsole() const
	{
	}

	const char* Scope::VariableAlreadyExists::ErrorType() const
	{
		return "Variable Already Exists";
	}

	void Scope::VariableDoesntExist::PrintToConsole() const
	{
	}

	const char* Scope::VariableDoesntExist::ErrorType() const
	{
		return "Variable Doesn't exist";
	}
}
