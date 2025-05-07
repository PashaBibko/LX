#pragma once

#include <LX-Common.h>

#include <ParserInfo.h>
#include <Lexer.h>

namespace LX
{
	// Thrown if there was an error during IR Generation //
	CREATE_EMPTY_LX_ERROR_TYPE(IRGenerationError);

	// Error thrown if the user tried to create a variable that already existed //
	CREATE_EMPTY_LX_ERROR_TYPE(VariableAlreadyExists);

	// Error thrown if user tries to access variable that does not exist //
	CREATE_EMPTY_LX_ERROR_TYPE(VariableDoesntExist);

	// Thrown if there was an unexpected (incorrect) token //
	struct UnexpectedToken : public RuntimeError
	{
		GENERATE_LX_ERROR_REQUIRED_FUNCTION_DECLARATIONS;

		// Constructor to set the members of the error //
		UnexpectedToken(Token::TokenType _expected, const ParserInfo& p);

		// Constructor for custom messages in the cmd //
		UnexpectedToken(Token::TokenType _expected, Token _got, const std::string& message, const ParserInfo& p);

		// The file that the tokens come from //
		const std::filesystem::path file;

		// The token type that should be there //
		const Token::TokenType expected;

		// If there are multiple expected types there is an option for a custom message //
		const std::string custom;

		// What token was actually at that position //
		// Stored as Token not TokenType to store the location of it within the source //
		const Token got;
	};
}
