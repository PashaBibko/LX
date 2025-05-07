#include <LX-Common.h>

#include <Parser.h>

#include <ParserErrors.h>
#include <ParserInfo.h>
#include <AST.h>

namespace LX
{
	// Util function for working out if a token is a two sided operator //
	static bool IsTwoSidedOperator(Token::TokenType t)
	{
		switch (t)
		{
			case Token::ADD:
			case Token::SUB:
			case Token::DIV:
			case Token::MUL:
				return true;

			default:
				return false;
		}
	}

	// Base of the call stack to handle the simplest of tokens //
	static std::unique_ptr<AST::Node> ParsePrimary(ParserInfo& p)
	{
		// There are lots of possible token's that can be here so a switch is used //
		switch (p.tokens[p.index].type)
		{
			// Number literals just require them to be turned into an AST node //
			// Note: Number literals are stored as strings because i'm a masochist //
			case Token::NUMBER_LITERAL:
				return std::make_unique<AST::NumberLiteral>(p.tokens[p.index++].GetContents());

			// If an Identifier has got here it means a variable is being accessed //
			case Token::IDENTIFIER:
				return std::make_unique<AST::VariableAccess>(p.tokens[p.index++].GetContents());

			// TODO: Fix this //
			case Token::OPEN_BRACKET:
				p.scopeDepth++;
				p.index++;
				return nullptr;

			// TODO: Fix this //
			case Token::CLOSE_BRACE:
				ThrowIf<UnexpectedToken>(p.scopeDepth == 0, Token::UNDEFINED, p.tokens[p.index], "need a different error", p);
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
	static std::unique_ptr<AST::Node> ParseOperation(ParserInfo& p)
	{
		// Checks if the next token is an operator //
		// TODO: Add more than just add //
		if (p.index + 1 < p.len) [[likely]]
		{
			if (IsTwoSidedOperator(p.tokens[p.index + 1].type))
			{
				// Parses the left hand side of the operation //
				std::unique_ptr<AST::Node> lhs = ParsePrimary(p);
				ThrowIf<UnexpectedToken>(lhs == nullptr, Token::UNDEFINED, p.tokens[p.index - 1], "value", p);

				// Stores the operator to pass into the AST node //
				Token::TokenType op = p.tokens[p.index].type;
				p.index++;

				// Parses the right hand of the operation //
				std::unique_ptr<AST::Node> rhs = ParseOperation(p);
				ThrowIf<UnexpectedToken>(rhs == nullptr, Token::UNDEFINED, p.tokens[p.index - 1], "value", p);

				// Returns an AST node as all of the components combined together //
				return std::make_unique<AST::Operation>(std::move(lhs), op, std::move(rhs));
			}
		}

		// Else goes down the call stack //
		return ParsePrimary(p);
	}

	// Handles return statements, if not calls ParseOperation //
	static std::unique_ptr<AST::Node> ParseReturn(ParserInfo& p)
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

	// Handles variable declarations, if not calls ParseReturn //
	static std::unique_ptr<AST::Node> ParseVarDeclaration(ParserInfo& p)
	{
		// Checks if the current token is a declaration //
		if (p.tokens[p.index].type == Token::INT_DEC)
		{
			// Skips over the dec token //
			p.index++;

			// Checks for the variable name //
			ThrowIf<UnexpectedToken>(p.tokens[p.index].type != Token::IDENTIFIER, Token::IDENTIFIER, p);
			std::string name = p.tokens[p.index].GetContents();
			p.index++; // <- Goes over the identifier token

			// Returns the declaration if there is no default assignment to the variable // 
			if (p.tokens[p.index].type != Token::ASSIGN)
			{
				// Creates the variable name from the contents of the token and returns it //
				return std::make_unique<AST::VariableDeclaration>(name);
			}

			p.index++; // Skips over Token::ASSIGN

			// Gets the value to be assigned to the variable //
			std::unique_ptr<AST::Node> defaultVal = ParsePrimary(p);
			ThrowIf<UnexpectedToken>(defaultVal.get() == nullptr, Token::UNDEFINED, p.tokens[p.index - 1], "value", p);

			return std::make_unique<AST::VariableDeclaration>(name);
		}

		// Else goes down the call stack //
		return ParseReturn(p);
	}

	// Handles variable assignments, if not calls ParseVarDeclaration //
	static std::unique_ptr<AST::Node> ParseVarAssignment(ParserInfo& p)
	{
		// Checks if the next token is an equals //
		if (p.index + 1 < p.len) [[likely]]
		{
			if (p.tokens[p.index + 1].type == Token::ASSIGN)
			{
				// Gets the variable that is being assigned too //
				ThrowIf<UnexpectedToken>(p.tokens[p.index].type != Token::IDENTIFIER, Token::IDENTIFIER, p);
				std::string name = p.tokens[p.index].GetContents();

				// Skips over the assign token and name of the variable //
				p.index = p.index + 2;

				// Gets the value that is being assigned //
				std::unique_ptr<AST::Node> value = ParseOperation(p);

				// Returns an AST node of the variable assignment with it's name and value //
				return std::make_unique<AST::VariableAssignment>(name, std::move(value));
			}
		}

		// Else goes down the call stack //
		return ParseVarDeclaration(p);
	}
	
	// Helper function to call the top of the Parse-Call-Stack //
	static inline std::unique_ptr<AST::Node> Parse(ParserInfo& p)
	{
		// Parses the current token //
		std::unique_ptr<AST::Node> out = ParseVarAssignment(p);

		// Checks it is valid before returning //
		ThrowIf<UnexpectedToken>(out == nullptr, Token::UNDEFINED, p.tokens[p.index - 1], "top level statement", p);
		return out;
	}

	// Turns the tokens of a file into it's abstract syntax tree equivalent //
	FileAST TurnTokensIntoAbstractSyntaxTree(std::vector<Token>& tokens, const std::filesystem::path& path)
	{
		// Logs the start of the parsing
		Log::LogNewSection("Started parsing tokens"); 

		// Creates the output storer and the parser //
		FileAST output;
		ParserInfo p(tokens, path);

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
					ThrowIf<UnexpectedToken>(p.tokens[p.index].type != Token::IDENTIFIER, Token::IDENTIFIER, p);
					func.name = p.tokens[p.index++].GetContents();

					// Checks for opening paren '(' //
					ThrowIf<UnexpectedToken>(p.tokens[p.index].type != Token::OPEN_PAREN, Token::OPEN_PAREN, p);
					p.index++;

					// Loops over all the arguments of the function //
					// TODO: Do something with the parameters
					while (p.index < p.len && (p.tokens[p.index].type == Token::CLOSE_PAREN) == false)
					{
						p.index++;
					}

					// Skips over close bracket //
					p.index++;

					// Checks for opening bracket '{' //
					ThrowIf<UnexpectedToken>(p.tokens[p.index].type != Token::OPEN_BRACKET, Token::OPEN_BRACKET, p);
					p.index++;

					// Loops over the body until it reaches the end //
					while (p.index < p.len && (p.tokens[p.index].type == Token::CLOSE_BRACKET && p.scopeDepth == 0) == false)
					{
						// Actually parses the function //
						std::unique_ptr<AST::Node> node = Parse(p);

						// Expands the node if it contains multiple //
						if (node->m_Type == AST::Node::MULTI_NODE)
						{
							for (std::unique_ptr<AST::Node>& containedNode : ((AST::MultiNode*)node.get())->nodes)
							{
								// Logs the node to the log //
								node->Log(0);

								// Adds it to the vector //
								func.body.push_back(std::move(containedNode));
							}
						}

						// Else adds the singular node to the vector //
						else
						{
							// Logs the node to the log //
							node->Log(0);

							// Adds it to the vector //
							func.body.push_back(std::move(node));
						}
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
		Log::out("AST length: ", output.functions[0].body.size());

		// Returns the output and shrinks all uneccesarry allocated memory
		output.functions.shrink_to_fit();
		return output;
	}
}
