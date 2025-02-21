#pragma once

#include <Parser/inc/FuncAST.h>

#include <Lexer/inc/Lexer.h>

namespace LX
{
	class Parser
	{
		private:
			FunctionAST ParseFunction(TokenSection<FuncToken>& section)
			{
			}

		public:
			Parser(std::vector<EmptyTokenSection>& tokens)
			{
			}
	};
}
