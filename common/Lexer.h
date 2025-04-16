#pragma once

#include <fstream>
#include <vector>
#include <string>

// This file contains everything that is exported from Lexer.lib
// The rest of the items within the Lexer project are internal only

namespace LX
{
	struct InvalidCharInSource
	{
		std::streamsize index;
		char invalid;
	};

	// Data type to store a more computer readable version of files
	struct __declspec(novtable) Token final
	{
		// Enum to hold the type of the token
		enum TokenType : short
		{
			// General tokens //

			STRING_LITERAL,
			IDENTIFIER,

			// Keywords //

			FOR, WHILE,
			IF, ELSE, ELIF,

			FUNCTION,

			// You made a mistake somehow //

			UNDEFINED = -1
		};

		// Constructor of the tokens to set their info
		Token(const TokenType _type, std::string _contents);

		// Contents of the token (may be empty if not needed)
		// Const to avoid external changes
		const std::string contents;

		// Type of the token
		// Const to avoid external changes
		const TokenType type;
	};

	// Lexer function to take in a file and output a vector of tokens
	const std::vector<Token> LexicalAnalyze(std::ifstream& src, std::ofstream* log);
}
