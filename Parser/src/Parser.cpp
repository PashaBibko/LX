#include <Parser.h>

#include <Util.h>
#include <AST.h>

#include <iostream>

namespace LX
{
	// Local struct so everything can be public //
	struct Parser
	{
		// Passes constructor args to members //
		Parser(std::vector<Token>& _tokens, std::ofstream* _log)
			: tokens(_tokens), log(_log), index(0), len(_tokens.size()), scopeDepth(0)
		{}

		// Tokens created by the lexer //
		std::vector<Token>& tokens;

		// Log to output to (can be null) //
		std::ofstream* log;

		// Length of the the token vector //
		const size_t len;

		// Current index within the token vector //
		size_t index;

		// Current scope depth //
		size_t scopeDepth;
	};

	// Base of the call stack to handle the simplest of tokens //
	static std::unique_ptr<AST::Node> ParsePrimary(Parser& p)
	{
		// There are lots of possible token's that can be here so a switch is used //
		switch (p.tokens[p.index].type)
		{
			// Number literals just require them to be turned into an AST node //
			// Note: Number literals are stored as strings because i'm a masochist //
			case Token::NUMBER_LITERAL:
				return std::make_unique<AST::NumberLiteral>(p.tokens[p.index++].GetContents());

			//
			case Token::OPEN_BRACKET:
				p.scopeDepth++;
				p.index++;
				return nullptr;

			case Token::CLOSE_BRACE:
				ThrowIf<UnexpectedToken>(p.scopeDepth == 0, Token::UNDEFINED, "need a different error", p.tokens[p.index]);
				p.scopeDepth--;
				p.index++;
				return nullptr;

			// Returns nullptr, the parsing function that recives that value will decide if that is valid //
			default:
				p.index++;
				return nullptr;
		}
	}

	// Handles operations, if it is not currently at an operation goes to ParsePrimary //
	static std::unique_ptr<AST::Node> ParseOperation(Parser& p)
	{
		// Checks if the next token is an operator //
		// TODO: Add more than just add //
		// TODO: Make this not crash when at the end //
		if (p.index + 1 < p.len) [[likely]]
		{
			if (p.tokens[p.index + 1].type == Token::ADD)
			{
				// Parses the left hand side of the operation //
				std::unique_ptr<AST::Node> lhs = ParsePrimary(p);
				ThrowIf<UnexpectedToken>(lhs == nullptr, Token::UNDEFINED, "value", p.tokens[p.index - 1]);

				// Stores the operator to pass into the AST node //
				Token::TokenType op = p.tokens[p.index].type;
				p.index++;

				// Parses the right hand of the operation //
				std::unique_ptr<AST::Node> rhs = ParseOperation(p);
				ThrowIf<UnexpectedToken>(rhs == nullptr, Token::UNDEFINED, "value", p.tokens[p.index - 1]);

				// Returns an AST node as all of the components combined together //
				return std::make_unique<AST::Operation>(std::move(lhs), op, std::move(rhs));
			}
		}

		// Else goes down the call stack //
		return ParsePrimary(p);
	}

	// Handles return statements, if not calls ParseOperation //
	static std::unique_ptr<AST::Node> ParseReturn(Parser& p)
	{
		// Checks if the current token is a return //
		if (p.tokens[p.index].type == Token::RETURN)
		{
			// If so it adds an AST node with the value being returned //
			// Does not mind if this returns nullptr as that just means nothing was returned //
			p.index++;
			return std::make_unique<AST::ReturnStatement>(ParseOperation(p));
		}

		// Else goes down the call stack //
		return ParseOperation(p);
	}
	
	// Helper function to call the top of the Parse-Call-Stack //
	static inline std::unique_ptr<AST::Node> Parse(Parser& p)
	{
		// Parses the current token //
		std::unique_ptr<AST::Node> out = ParseReturn(p);

		// Checks it is valid before returning //
		ThrowIf<UnexpectedToken>(out == nullptr, Token::UNDEFINED, "top level statement", p.tokens[p.index - 1]);
		return out;
	}

	// Turns the tokens of a file into it's abstract syntax tree equivalent //
	FileAST TurnTokensIntoAbstractSyntaxTree(std::vector<Token>& tokens, std::ofstream* log)
	{
		// Logs the start of the parsing
		SafeLog(log, LOG_BREAK, "Started parsing tokens", LOG_BREAK);

		// Creates the output storer and the parser //
		FileAST output;
		Parser p(tokens, log);

		// Loops over the tokens and calls the correct parsing function //
		// Which depends on their type and current state of the parser //
		while (p.index < p.len)
		{
			switch (p.tokens[p.index].type)
			{
				case Token::FUNCTION:
				{
					// Skips over function token //
					p.index++;

					// Pushes a new function to the vector and gets a reference to it for adding the body //
					output.functions.emplace_back();
					FunctionDefinition& func = output.functions.back();

					// Assigns the function name //
					ThrowIf<UnexpectedToken>(p.tokens[p.index].type != Token::IDENTIFIER, Token::IDENTIFIER, "", p.tokens[p.index]);
					func.name = p.tokens[p.index++].GetContents();

					// Checks for opening bracket '{' //
					ThrowIf<UnexpectedToken>(p.tokens[p.index].type != Token::OPEN_BRACKET, Token::OPEN_BRACKET, "", p.tokens[p.index]);
					p.index++;

					// Loops over the body until it reaches the end //
					// TODO: Detect the end instead of looping over the entire token vector
					while (p.index < p.len && (p.tokens[p.index].type == Token::CLOSE_BRACKET && p.scopeDepth == 0) == false)
					{
						// Actually parses the function
						std::unique_ptr<AST::Node> node = Parse(p);

						// Logs the node to the log //
						SafeLog(log, ToString(node));

						// Adds it to the vector
						func.body.push_back(std::move(node));
					}

					// Skips over closing bracket //
					p.index++;

					// Goes to the next iteration of the loop //
					continue;
				}

				// Lets the user know there is an error //
				// TODO: Makes this error actually output useful information //
				default:
					std::cout << "UNKNOWN TOKEN FOUND: " << ToString(p.tokens[p.index].type) << std::endl;
					return output;
			}
		}

		// Logs that AST has finished parsing //
		// TODO: Make this output the AST in a human-readable form //
		SafeLog(log, "AST length: ", output.functions[0].body.size());

		// Returns the output and shrinks all uneccesarry allocated memory
		output.functions.shrink_to_fit();
		return output;
	}
}
