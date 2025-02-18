#pragma once

#include <Common/inc/Poly.h>

#include <string_view>

class Lexer;

enum class SectionType
{
	LX_FUNCTION,

	UNDEFINED = 0
};

class SourceSection final : public PolyBase
	<SectionType, SectionType::UNDEFINED>
{
	private:
		// Declares what the section contains (inputs, outputs, name)
		std::string_view m_Declaration;

		// Contents of the section (input -> output)
		std::string_view m_Definition;

		// The lexer that created this section
		// Not static to allow for easier multithreading
		Lexer& m_Creator;

	public:
		// Constructor
		SourceSection(Lexer& creator, std::string_view declaration, std::string_view definition);

		// Gets the section decleration
		inline const std::string_view& GetDeclaration() const { return m_Declaration; }

		// Gets the section definition
		inline const std::string_view& GetDefinition() const { return m_Definition; }
};
