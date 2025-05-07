#include <LX-Common.h>

#include <Lexer.h>

#include <LexerInfo.h>

namespace LX
{
	// Passes the constructor args to the values //
	Token::Token(const TokenType _type, const LexerInfo& info, std::streamsize _length, const std::string& source)
		: type(_type), index(info.index - _length + 1), line(info.line), column(info.column - _length), length(_length), contents(source.data() + index, length)
	{}

	// This function used to have a use //
	// Now it exists cause I'm lazy //
	std::string Token::GetContents() const
	{
		return contents;
	}
}
