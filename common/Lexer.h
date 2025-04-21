#pragma once

#include <vector>
#include <string>

// Foward declarations of STD classes to minimise includes //
namespace std
{
	template<typename T1>
	struct char_traits;

	template<typename T1, typename T2>
	class basic_ofstream;

	using ofstream = basic_ofstream<char, char_traits<char>>;
}

// This file contains everything that is exported from Lexer.lib
// The rest of the items within the Lexer project are internal only

namespace LX
{
	// Error type with index and character to alert the user that LX does not understand that symbol //
	struct InvalidCharInSource
	{
		std::streamsize col;
		std::streamsize line;
		std::streamsize index;

		char invalid;
	};

	// Struct to store the current information of the lexer //
	struct LexerInfo
	{
		// Current trackers of where in the source it is //

		std::streamsize line = 1; // <- Lines start on 1 (probably because of non-programmer's)
		std::streamsize index = 0;
		std::streamsize column = 0; // <- Columns start on 1 (probably because of non-programmer's)

		// Trackers for when a multi-char token started //

		std::streamsize startOfWord = 0;
		std::streamsize startOfNumberLiteral = 0;
		std::streamsize startOfStringLiteral = 0;

		// Different flags of the lexer //
		// Stored as a bitset to minimse memory allocated (basically no difference, because only one exists at any given time) //

		bool isAlpha : 1 = false;
		bool isNumeric : 1 = false;
		bool inComment : 1 = false;
		bool inStringLiteral : 1 = false;
		bool isNextCharAlpha : 1 = false;
		bool isNextCharNumeric : 1 = false;
		bool wasLastCharAlpha : 1 = false;
		bool wasLastCharNumeric : 1 = false;
		bool lexingNumber : 1 = false;
	};

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
		Token(const TokenType _type, const LexerInfo& info, std::string _contents, std::streamsize _length);

		// Contents of the token (may be empty if not needed) //
		// Const to avoid external changes //
		const std::string contents;

		// Type of the token //
		// Const to avoid external changes //
		const TokenType type;
		
		// Index in the source of the token //
		const std::streamsize index;

		// The length of the token on the line, may be different to the length of contents //
		const std::streamsize length;

		// The line the token is located on //
		const std::streamsize line;

		// The column on the line where it is located //
		const std::streamsize column;
	};

	// Logging function to turn a tokentype enum val into it's string //
	std::string ToString(Token::TokenType t);
	
	// Lexer function to take in a file and output a vector of tokens //
	const std::vector<Token> LexicalAnalyze(const std::string& contents, const std::streamsize len, std::ofstream* log);
}
