#include <LX-Common.h>

#include <Lexer.h>

#include <LexerInfo.h>

namespace LX
{
	// Helper macro for the function below //
	// Undefined after the function to stop accidental use //
	#define TOKEN_CASE(type) case type: return #type;

	// Static to stop use outside of this file as ToString should be used instead //
	static std::string ToStringNoFormat(Token::TokenType type)
	{
		switch (type)
		{
			TOKEN_CASE(Token::STRING_LITERAL);
			TOKEN_CASE(Token::IDENTIFIER);
			TOKEN_CASE(Token::FOR);
			TOKEN_CASE(Token::WHILE);
			TOKEN_CASE(Token::IF);
			TOKEN_CASE(Token::ELSE);
			TOKEN_CASE(Token::ELIF);
			TOKEN_CASE(Token::FUNCTION);
			TOKEN_CASE(Token::ADD);
			TOKEN_CASE(Token::SUB);
			TOKEN_CASE(Token::MUL);
			TOKEN_CASE(Token::DIV);
			TOKEN_CASE(Token::NUMBER_LITERAL);
			TOKEN_CASE(Token::RETURN);
			TOKEN_CASE(Token::OPEN_BRACE);
			TOKEN_CASE(Token::CLOSE_BRACE);
			TOKEN_CASE(Token::OPEN_BRACKET);
			TOKEN_CASE(Token::CLOSE_BRACKET);
			TOKEN_CASE(Token::OPEN_PAREN);
			TOKEN_CASE(Token::CLOSE_PAREN);
			TOKEN_CASE(Token::ASSIGN);
			TOKEN_CASE(Token::INT_DEC);

			// Default just returns it as it's numerical value //
			default: return "Unknown: " + std::to_string((int)type);
		}
	}
	
	#undef TOKEN_CASE

	// Logging function to turn a tokentype enum val into a nicer formatted string than the base equivalent //
	std::string ToString(Token::TokenType type)
	{
		// Gets the unformated version of the string //
		std::string unformatted = ToStringNoFormat(type);
		unformatted = unformatted.substr(7); // Removes the Token:: prefix

		// Formats the string (turns to lowercase and replaces _ with a space //
		std::string formatted;

		for (char current : unformatted)
		{
			// Adding 32 makes it lowercase due to how ASCII works //
			if ((current >= 'A' && current <= 'Z')) { formatted.push_back(current + 32); }

			// Replaces _ with spaces //
			else if (current == '_') { formatted.push_back(' '); }

			// Else adds the current character //
			else { formatted.push_back(current); }
		}

		// Returns the formatted string //
		return formatted;
	}

	// Passes the constructor args to the values //
	Token::Token(const TokenType _type, const LexerInfo& info, std::streamsize _length, const std::string& source)
		: type(_type), index(info.index - _length + 1), line(info.line), column(info.column - _length), length(_length), contents(source.data() + index, length)
	{}

	// This function used to have a use but now it is just a simple getter //
	// Recommended to use in case of future changes //
	std::string Token::GetContents() const
	{
		return contents;
	}
}
