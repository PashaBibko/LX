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

	UnexpectedToken::UnexpectedToken(Token::TokenType _expected, std::string _override, Token _got, const std::filesystem::path& _file)
		: expected(_expected), override(_override), got(_got), file(_file)
	{}

	void UnexpectedToken::PrintToConsole() const
	{
		// Calculates the length of the line number in the console so it is formatted correctly //
		std::ostringstream oss;
		oss << std::setw(3) << got.line;
		size_t lineNumberWidthInConsole = std::max(oss.str().size(), (size_t)3);

		// Gets the line of the error //
		// As the file has been closed and the source has been deleted it needs to be reopened //

		std::string fileContents = ReadFileToString(file);
		std::string line = LX::GetLineAtIndexOf(fileContents, got.index);

		// Prints the error to the console with the relevant info //
		std::cout << "\n";
		PrintAsColor<Color::LIGHT_RED>("Error: ");
		std::cout << "Incorrect syntax in ";
		PrintAsColor<Color::WHITE>(file.string());
		std::cout << ", found ";
		PrintAsColor<Color::WHITE>(LX::ToString(got.type).c_str());
		std::cout << " expected: ";

		// Allows the error to have a custom type that is printed to the console //
		if (expected == LX::Token::UNDEFINED) { PrintAsColor<Color::WHITE>(override.c_str()); }
		else { PrintAsColor<Color::WHITE>(ToString(expected).c_str()); }
		std::cout << "\n";

		// Prints the code with the error to the console //
		std::string errorSquiggle(got.length, '~');
		std::cout << "Line: " << std::setw(lineNumberWidthInConsole) << got.line << " | " << line << "\n";
		std::cout << "      " << std::setw(lineNumberWidthInConsole) << "" << " | " << std::setw(got.column) << "";
		PrintAsColor<Color::LIGHT_RED>(errorSquiggle.c_str());
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
