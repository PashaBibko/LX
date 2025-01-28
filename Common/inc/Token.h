#pragma once

#include <Common/inc/Macro.h>

#include <string_view>
#include <string>

class Token final
{
	public:
		// Enum to keep track of the token's type
		enum class Type
		{
			// Default
			UNDEFINED = 0
		};

	private:
		// The type of token
		const Type m_Type = Type::UNDEFINED;

		// Contents of the token
		std::string_view m_Contents;

	public:
		// Constructor
		Token(const Type type, const std::string& str, size_t start, size_t len);

		// Deconstructor
		~Token() = default;

		// Returns unchangeable view of the string
		R_INL const std::string_view Str() const
		H_CONT_INL({ return m_Contents; })

		R_INL bool operator== (Type other) const
		H_CONT_INL({ return m_Type == other; })

		R_INL bool operator!= (Type other) const
		H_CONT_INL({ return m_Type != other; })
};
