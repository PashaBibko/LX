#pragma once

#include <Parser/inc/FuncAST.h>

#include <Lexer/inc/Lexer.h>

namespace LX
{
	struct FileAST
	{
		// Vector of all the functions within the file
		std::vector<FunctionAST> func;
	};

	class Parser
	{
		private:
			// Function for parsing functions
			FuncAST::Node ParseFunction(TokenSection<FuncToken>& section);

		public:
			// Constructor
			Parser(TokenOutput& tokens, FileAST& AST)
			{
				//
				for (TokenSection<FuncToken>& funcTokenSect : tokens.funcTokens)
				{
					// Pushes the new FunctionAST to the vector
					FunctionAST& func = AST.func.emplace_back(funcTokenSect.DecTokens());

					// Creates the linked list of the body
					func.body = ParseFunction(funcTokenSect);
				}
			}
	};
}
