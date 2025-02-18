#include <Lexer/inc/Lexer.h>

#include <stdexcept>

namespace LX
{
	Lexer::Lexer(const std::string& src)
		: m_Source(src), m_SourceLen(src.length())
	{
		// Checks source is a valid length
		if (m_SourceLen == 0x0)
		{
			throw std::runtime_error("Source cannot be empty");
		}

		// Splits the source into sections
		Split();

		#ifdef LEXER_SPLITTER_LOGGING

		// Prints the blocks to the console
		for (int i = 0; i < m_Sections.size(); i++)
		{
			LOG_BREAK;
			LOG("Section: " << i);

			LOG_BREAK;
			LOG("Declaration:\n" << m_Sections[i].GetDeclaration());

			LOG_BREAK;
			LOG("Definition:\n" << m_Sections[i].GetDefinition());
		}

		#endif // LEXER_SPLITTER_LOGGING
	}
}
