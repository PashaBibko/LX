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
			IDENTIFIER, // Any word

			OPEN_BRACKET, // (
			CLSE_BRACKET, // )

			OPEN_CROCK, // <
			CLSE_CROCK, // >

			UNDEFINED = -1
		};

		enum class Func : short
		{
			IDENTIFIER,

			UNDEFINED = -1
		};
	}

	// Default type for tokens to inherit from
	template<typename TokenEnum, TokenEnum defaultVal>
	struct __declspec(novtable) TokenBase
	{
		// Constructor for error checking
		explicit TokenBase(const TokenEnum type, std::string_view contents)
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
		explicit DecToken(TokenTypes::Dec type, std::string_view contents = std::string_view{})
			: TokenBase(type, contents) {}
	};

	// Tokens used to specify contents of a function
	struct __declspec(novtable) FuncToken
		: TokenBase<TokenTypes::Func, TokenTypes::Func::UNDEFINED>
	{
		// Inherits from constructor for error checking
		explicit FuncToken(TokenTypes::Func type, std::string_view contents = std::string_view{})
			: TokenBase(type, contents) {}
	};
}
