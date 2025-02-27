#include <Parser/inc/Parser.h>

#include <Parser/inc/FuncAST.h>

#include <Lexer/inc/Lexer.h>

namespace LX
{
	namespace Token = TokenTypes;
	namespace AST = FuncAST;

	struct ParserInfo
	{
		// The tokens to parse
		std::vector<FuncToken>& tokens;

		// Index within the token section
		size_t index;

		// The list of nodes
		FuncAST::Node list;

		// Constructor
		explicit ParserInfo(std::vector<FuncToken>& _tokens)
			: tokens(_tokens), index(0), list(nullptr)
		{}
	};

	static void ParsePrimary(ParserInfo& info)
	{
		// Switches over the enum type
		switch (info.tokens[info.index].m_Type)
		{
			case Token::Func::IDENTIFIER:
				info.list.Push<AST::Identifier>(info.tokens[info.index].m_Contents);
				info.index++;
				break;

			default:
				LOG("Unknown token type: " << (short)info.tokens[info.index].m_Type);
				info.index++;
				break;
		}
	}

	static void ParseOperation(ParserInfo& info)
	{
		// Calls next function in the stack
		ParsePrimary(info);
	}

	static void ParseFuncCall(ParserInfo& info)
	{
		// Calls next function in the stack
		ParseOperation(info);
	}

	static void ParseAssignment(ParserInfo& info)
	{
		// Calls next function in the stack
		ParseFuncCall(info);
	}

	static void ParseVarDeclaration(ParserInfo& info)
	{
		// Calls next function in the stack
		ParseAssignment(info);
	}

	static void ParseBranch(ParserInfo& info)
	{
		// Calls next function in the stack
		ParseVarDeclaration(info);
	} 

	FuncAST::Node Parser::ParseFunction(TokenSection<FuncToken>& section)
	{
		// Creates the information for the parser functions
		ParserInfo info(section.ContentsToken());

		// Length of the token vector
		size_t len = info.tokens.size();

		// Iterates over the tokens until complete
		while (info.index < len)
		{
			// Calls function at the top of the call stack	
			ParseBranch(info);
		}

		// Returns the root of the list
		return info.list.AsList();
	}
}
