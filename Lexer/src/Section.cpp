#include <Lexer/inc/Section.h>

static constexpr bool IsWhitespace(const char c)
{
	return c == ' '  || 
		   c == '\t' ||
		   c == '\n' ||
		   c == '\r';
}

SourceSection::SourceSection(Lexer& creator, std::string_view declaration, std::string_view definition)
	: m_Creator(creator), m_Declaration(declaration), m_Definition(definition)
{
	// Gets start of declaration excluding whitespace
	size_t decStart = std::find_if(m_Declaration.begin(), m_Declaration.end(), [](char c) { return !IsWhitespace(c); }) - m_Declaration.begin();

	// Gets start of definition excluding whitespace
	size_t defStart = std::find_if(m_Definition.begin(), m_Definition.end(), [](char c) { return !IsWhitespace(c); }) - m_Definition.begin();

	// Trims the declaration and definition
	m_Declaration = m_Declaration.substr(decStart);
	m_Definition = m_Definition.substr(defStart);
}
