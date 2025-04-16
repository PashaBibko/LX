#include <Lexer.h>

#include <string>
#include <vector>

namespace LX
{
	Token::Token(const TokenType _type, std::string _contents)
		: type(_type), contents(_contents)
	{}
}
