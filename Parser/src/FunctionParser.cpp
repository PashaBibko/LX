#include <Parser/inc/Parser.h>

#include <Parser/inc/FuncAST.h>

#include <Lexer/inc/Lexer.h>

#include <array>

namespace LX
{
	namespace Token = TokenTypes;
	namespace AST = FuncAST;

	enum class ParserState
	{
		BRANCH,
		DECLARATION,
		ASSIGN,
		FUNC_CALL,
		OPERATION,
		PRIMARY
	};

	struct ParserInfo
	{
		// The tokens to parse
		std::vector<FuncToken>& tokens;

		// Index within the token section
		size_t index;

		// The list of nodes
		FuncAST::Node list;

		// The state of the parser
		ParserState state;

		// Constructor
		explicit ParserInfo(std::vector<FuncToken>& _tokens)
			: tokens(_tokens), index(0), list(nullptr), state(ParserState::BRANCH)
		{}
	};

	static AST::Node ParseBracketExpression(ParserInfo& info)
	{
		//return AST::Node(nullptr);
	}

	static bool ParsePrimary(ParserInfo& info)
	{
		// Switches over the enum type
		switch (info.tokens[info.index].m_Type)
		{
			case Token::Func::IDENTIFIER:
				info.list.Push<AST::Identifier>(info.tokens[info.index].m_Contents);
				info.index++;
				return false;

			//case Token::Func::OPEN_PAREN:
				//info.list.Push<AST::BracketExpression>(info);
				//break;

			default:
				LOG("Unknown token type: " << (short)info.tokens[info.index].m_Type);
				info.index++;
				return false;
		}
	}

	static bool ParseOperation(ParserInfo& info)
	{
		LOG("");
		info.index++;
		return false;
	}

	static bool ParseFuncCall(ParserInfo& info)
	{
		LOG("");
		return true;
	}

	static bool ParseAssignment(ParserInfo& info)
	{
		LOG("");
		return true;
	}

	static bool ParseVarDeclaration(ParserInfo& info)
	{
		LOG("");
		return true;
	}

	static bool ParseBranch(ParserInfo& info)
	{
		//
		info.st

		LOG("");
		return true;
	} 

	FuncAST::Node Parser::ParseFunction(TokenSection<FuncToken>& section)
	{
		// Creates the information for the parser functions
		ParserInfo info(section.ContentsToken());

		// Length of the token vector
		size_t len = info.tokens.size();

		// Alias for easier function pointer usage
		// Returns bool and takes in a reference to parser info
		using ParseFunction = bool(*)(ParserInfo&);

		// Array of all the parsing functions to iterate over
		static const std::array<ParseFunction, 6> funcs
		{{
			ParseBranch,
			ParseVarDeclaration,
			ParseAssignment,
			ParseFuncCall,
			ParseOperation,
			ParsePrimary
		}};

		// Iterates over the tokens until complete
		while (info.index < len)
		{
			// Wether to call the current function
			bool call = false;

			// Index in the array
			size_t index = 0;

			// Iterates over the array
			for (ParseFunction func : funcs)
			{
				// Works out wether to call the current function depending on the state
				// Always true if it was already true
				call = (index == (size_t)info.state) || (call);

				// Calls the function from the array 
				if (call) { call = func(info); }

				// Iterate to the next index
				index++;
			}
		}

		// Returns the root of the list
		return info.list.AsList();
	}
}
