#pragma once

#include <Parser/inc/FuncAST.h>

#include <Lexer/inc/Lexer.h>

#include <iostream>

namespace LX
{
	static void LogFuncNode(const FuncAST::Node& node, std::string startMessage = "", int depth = 0)
	{
		using namespace FuncAST;

		for (int i = 0; i < depth; i++)
		{
			std::cout << '\t';
		}

		std::cout << startMessage;

		switch (node.CurrentType())
		{
			case NodeType::IDENTIFIER:
				LOG("Identifier: " << node.as<Identifier>()->m_Name);
				return;

			case NodeType::VAR_DEC:
				LOG("Declaration:");
				LogFuncNode(Node(node.as<Declaration>()->m_Name), "Name: ", depth + 1);
				LogFuncNode(Node(node.as<Declaration>()->m_Type), "Type: ", depth + 1);
				return;

			case NodeType::VAR_ASSIGN: 
				LOG("Assignment:");
				LogFuncNode(Node(node.as<Assignment>()->m_Asignee), "Asignee: ", depth + 1);
				LogFuncNode(Node(node.as<Assignment>()->m_Value), "Value: ", depth + 1);
				return;

			case NodeType::OPERATION:
				LOG("Operation: {" << (short)node.as<Operation>()->m_Operand << "}");
				LogFuncNode(Node(node.as<Operation>()->m_Lhs), "Left: ", depth + 1);
				LogFuncNode(Node(node.as<Operation>()->m_Rhs), "Right: ", depth + 1);
				return;
		}
	}

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
				// Iterates over the token sections and adds the functions AST to the vector
				for (TokenSection<FuncToken>& funcTokenSect : tokens.funcTokens)
				{
					// Pushes the new FunctionAST to the vector
					FunctionAST& func = AST.func.emplace_back(funcTokenSect.DecTokens());

					// Creates the linked list of the body
					func.body = ParseFunction(funcTokenSect);

					// Logging
					#ifdef PARSER_AST_LOGGING
					
					FuncAST::Node node = func.body;

					// Loops over the list and logs each node
					while (node)
					{
						LogFuncNode(node);

						node.Next();
					}

					#endif // PARSER_AST_LOGGING
				}
			}
	};
}
