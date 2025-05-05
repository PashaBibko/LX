#include <LX-Common.h>

#include <Lexer.h>

namespace LX
{
	// Creates the memory for the pointer to the source //
	std::string* Token::source = nullptr;

	// Passes the constructor args to the values //
	Token::Token(const TokenType _type, const LexerInfo& info, std::streamsize _length)
		: type(_type), index(info.index - _length + 1), line(info.line), column(info.column - _length), length(_length)
	{}

	//
	std::string Token::GetContents() const
	{
		return std::string(source->data() + index, length);
	}
}
