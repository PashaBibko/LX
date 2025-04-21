#include <Lexer.h>

#include <string>
#include <vector>

namespace LX
{
	// Passes the constructor args to the values //
	Token::Token(const TokenType _type, std::string _contents, std::streamsize _line, std::streamsize _index, std::streamsize _length)
		: type(_type), contents(_contents), line(_line), index(_index), length(_length)
	{}
}
