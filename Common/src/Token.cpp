#include <Common/inc/Token.h>

#include <stdexcept>

Token::Token(const TokenType type, const std::string& str, size_t start, size_t len)
{
	// Checks the start of token is inside string bounds
	if (start >= str.size())
	{
		throw std::runtime_error("Start of token is outside string bounds");
	}

	// Checks token length does not overflow string
	if (std::min(len, str.size() - start) != len)
	{
		throw std::runtime_error("Token length overflows given string");
	}

	// Assigns the string view
	m_Contents = std::string_view(str.data() + start, len);
}
