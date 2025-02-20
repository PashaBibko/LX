#pragma once

#include <Common/Poly.h>

#include <string_view>
#include <stdexcept>
#include <string>

namespace LX
{
	namespace TokenTypes
	{
		enum class Dec : short
		{
			// Any non keyword-word
			IDENTIFIER,

			// Block types
			FUNCTION,

			// ()
			OPEN_BRACKET, CLSE_BRACKET,

			// <>
			OPEN_CROCK, CLSE_CROCK,

			COMMA,

			UNDEFINED = -1
		};

		enum class Func : short
		{
			// Any non-keyword word
			IDENTIFIER,

			// Variables //

			// Declaration
			INT_DEC, STR_DEC,

			// Literals
			STRING_LITERAL, NUMBER_LITERAL,

			// Variable modifiers
			CONSTANT, REFERENCE, POINTER,

			// Control flow modifiers //

			// Simple branching
			IF, ELSE_IF, ELSE,

			// Loops
			WHILE, FOR, FOR_EACH,

			// Go-to statements
			BREAK, CONTINUE, RETURN,

			// Math operators //

			// Arethmetic
			ADD, SUB, MUL, DIV, MOD,

			// Compound Arethmetic
			C_ADD, C_SUB, C_MUL, C_DIV, C_MOD,

			// Increment and decrement
			INCREMENT, DECREMENT,

			// Logic operators //

			// Equality
			EQUAL, N_EQUAL,

			// Greater than
			GREATER_THAN, GREATER_THAN_EQUALS,

			// Less than
			LESS_THAN, LESS_THAN_EQUALS,

			//
			AND, OR, NOT,

			// Programming operators //

			// General use operators
			COMMA, SEMI_COLON, COLON, ASSIGN,

			// Member of operators
			DOT, ARROW, DOUBLE_ARROW, DOUBLE_COLON,

			// BRACKETS //

			// ()
			OPEN_PAREN, CLOSE_PAREN,

			// []
			OPEN_BRACK, CLOSE_BRACK,

			// {}
			OPEN_BRACE, CLOSE_BRACE,

			UNDEFINED = -1
		};
	}

	// Default type for tokens to inherit from
	template<typename TokenEnum, TokenEnum defaultVal>
	struct __declspec(novtable) TokenBase
	{
		// Constructor for error checking
		TokenBase(const TokenEnum type, std::string_view contents)
			: m_Type(type), m_Contents(contents)
		{
			// Throws error if type is default
			if (m_Type == defaultVal)
			{
				throw std::runtime_error("Token has unknown type: " + (short)m_Type);
			}
		}

		// Contents of the token (can be empty)
		std::string_view m_Contents;

		// Type of the token
		// Throws error if it is default type
		TokenEnum m_Type;
	};

	// Tokens used to specify contents of a block/section
	struct __declspec(novtable) DecToken
		: TokenBase<TokenTypes::Dec, TokenTypes::Dec::UNDEFINED>
	{
		// Inherits from constructor for error checking
		DecToken(TokenTypes::Dec type, std::string_view contents = std::string_view{})
			: TokenBase(type, contents) {}
	};

	// Tokens used to specify contents of a function
	struct __declspec(novtable) FuncToken
		: TokenBase<TokenTypes::Func, TokenTypes::Func::UNDEFINED>
	{
		// Inherits from constructor for error checking
		FuncToken(TokenTypes::Func type, std::string_view contents = std::string_view{})
			: TokenBase(type, contents) {}
	};
}
