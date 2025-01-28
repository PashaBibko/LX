#pragma once

#include <Common/inc/Poly.h>

#include <string_view>
#include <string>

enum class TokenType
{
	UNDEFINED = 0
};

class __declspec(novtable) Token final : public PolyBase
	<const TokenType, TokenType::UNDEFINED>
{
	private:
		// Contents of the token
		std::string_view m_Contents;

	public:
		// Constructor
		Token(const TokenType type, const std::string& str, size_t start, size_t len);

		// Deconstructor
		~Token() = default;

		// Returns unchangeable view of the string
		inline const std::string_view& Str() const
		{
			return m_Contents;
		}
};
