#pragma once

#include <LX-Common.h>

// This file contains everything that is exported from Lexer.lib
// The rest of the items within the Lexer project are internal only

namespace LX
{
	// Struct to store the current information of the lexer //
	struct LexerInfo;

	// Data type to store a more computer readable version of files
	struct __declspec(novtable) Token final
	{
		// Enum to hold the type of the token //
		enum TokenType : short
		{
			// General tokens //

			STRING_LITERAL,
			NUMBER_LITERAL,
			IDENTIFIER,
			RETURN,

			// Built-in types //

			INT_DEC,

			// Symbols //

			OPEN_BRACKET, CLOSE_BRACKET,
			OPEN_BRACE, CLOSE_BRACE,
			OPEN_PAREN, CLOSE_PAREN,

			COMMA,

			//

			ASSIGN,

			// Operators //

			ADD, SUB, MUL, DIV,

			// Keywords //

			FOR, WHILE,
			IF, ELSE, ELIF,

			FUNCTION,

			// You made a mistake somehow //

			UNDEFINED = -1
		};
	
		// Constructor of the tokens to set their info //
		Token(const TokenType _type, const LexerInfo& info, std::streamsize _length, const std::string& source);

		// Works out the contents of the token and returns them as it is not stored in the token //
		std::string GetContents() const;

		// Type of the token //
		// Const to avoid external changes //
		const TokenType type;
		
		// Index in the source of the token //
		const std::streamsize index;

		// The length of the token on the line //
		const std::streamsize length;

		// The line the token is located on //
		const std::streamsize line;

		// The column on the line where it is located //
		const std::streamsize column;

		// The contents of the token //
		const std::string contents;
	};

	// Logging function to turn a tokentype enum val into it's string //
	std::string ToString(Token::TokenType t);
	
	// Lexer function to take in a file and output a vector of tokens //
	const std::vector<Token> LexicalAnalyze(const std::filesystem::path& path);
}
