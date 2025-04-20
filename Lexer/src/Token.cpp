#include <Lexer.h>

#include <string>
#include <vector>

namespace LX
{
	// Passes the constructor args to the values //
	Token::Token(const TokenType _type, std::string _contents)
		: type(_type), contents(_contents)
	{}
}
