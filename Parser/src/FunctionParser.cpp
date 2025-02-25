#include <Parser/inc/Parser.h>

#include <Parser/inc/FuncAST.h>

#include <Lexer/inc/Lexer.h>

namespace LX
{
	FuncAST::Node Parser::ParseFunction(TokenSection<FuncToken>& section)
	{
		throw std::runtime_error("Parser::ParseFunction(TokenSection<FuncToken>& has not been implemented");
	}
}
