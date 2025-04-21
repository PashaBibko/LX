#include <Lexer.h>

#include <string>
#include <vector>

namespace LX
{
	// Passes the constructor args to the values //
	Token::Token(const TokenType _type, const LexerInfo& info, std::string _contents, std::streamsize _length)
		: type(_type), contents(_contents), index(info.index - _length + 1), line(info.line), column(info.column - _length), length(_length)
	{}
}
