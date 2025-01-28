#include <Common/inc/Token.h>

#include <stdexcept>

Token::Token(const Type type, const std::string& str, size_t start, size_t len)
	: m_Type(type)
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

C_DEF_INL(const std::string_view Token::Str() const { return m_Contents; })

C_DEF_INL(bool Token::operator== (Type other) const { return m_Type == other; })
C_DEF_INL(bool Token::operator!= (Type other) const { return m_Type != other; })
