#include <Parser.h>
#include <Util.h>

#include <iostream>

namespace LX
{
	// Local struct so everything can be public //
	struct Parser
	{
		Parser(std::vector<Token>& _tokens, std::ofstream* _log)
			: tokens(_tokens), log(_log), index(0), len(_tokens.size())
		{}

		std::vector<Token>& tokens;
		std::ofstream* log;

		const size_t len;
		size_t index;
	};

	static std::unique_ptr<AST::Node> ParsePrimary(Parser& p)
	{
		switch (p.tokens[p.index].type)
		{
			case Token::NUMBER_LITERAL:
				return std::make_unique<AST::NumberLiteral>(p.tokens[p.index].contents);

			default:
				std::cout << "UNKNOWN TOKEN: " << p.tokens[p.index].type << std::endl;
				return nullptr;
		}
	}

	static std::unique_ptr<AST::Node> ParseOperation(Parser& p)
	{
		if (p.tokens[p.index + 1].type == Token::ADD)
		{
			std::unique_ptr<AST::Node> lhs = ParsePrimary(p);
			p.index++;

			// Skips over operator (again i'm lazy)
			p.index++;

			std::unique_ptr<AST::Node> rhs = ParsePrimary(p);

			return std::make_unique<AST::Operation>(std::move(lhs), Token::ADD, std::move(rhs));
		}

		return ParsePrimary(p);
	}

	static std::unique_ptr<AST::Node> ParseReturn(Parser& p)
	{
		if (p.tokens[p.index].type == Token::RETURN)
		{
			p.index++;
			return std::make_unique<AST::ReturnStatement>(ParseOperation(p));
		}

		return ParseOperation(p);
	}

	static std::unique_ptr<AST::Node> Parse(Parser& p)
	{
		return ParseReturn(p);
	}

	FileAST TurnTokensIntoAbstractSyntaxTree(std::vector<Token>& tokens, std::ofstream* log)
	{
		// Logs the start of the parsing
		SafeLog(log, LOG_BREAK, "Started parsing tokens", LOG_BREAK);

		FileAST output;
		Parser p(tokens, log);

		while (p.index < p.len)
		{
			switch (p.tokens[p.index].type)
			{
				case Token::FUNCTION:
				{
					// Skips over function token + name token
					// TEMPORARY
					p.index++; p.index++;

					output.functions.emplace_back();
					FunctionDefinition& func = output.functions.back();

					// TEMPORARY casue im lazy
					while (p.index < p.len)
					{
						// Actually parses the function
						std::unique_ptr<AST::Node> node = Parse(p);

						// Adds it to the vector and iterates to the next token
						func.body.push_back(std::move(node));
						p.index++;
					}

					continue;
				}

				default:
					std::cout << "UNKNOWN TOKEN FOUND" << std::endl;
			}
		}

		SafeLog(log, "AST length: ", output.functions[0].body.size());

		return output;
	}
}
