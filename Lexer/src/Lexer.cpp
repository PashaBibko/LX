#include <Lexer/inc/Lexer.h>

#include <stdexcept>

namespace LX
{
	Lexer::Lexer(const std::string& src)
		: m_Source(src)
	{
		// Checks source is a valid length
		if (m_Source.length() == 0x0)
		{
			throw std::runtime_error("Source cannot be empty");
		}

		// Splits the source into sections
		std::vector<SourceSection> sections = Split();

		#ifdef LEXER_SPLITTER_LOGGING

		LOG_BREAK;
		LOG("Section Count: " << sections.size());

		// Prints the blocks to the console
		for (int i = 0; i < sections.size(); i++)
		{
			LOG_BREAK;
			LOG("Section: " << i);

			LOG_BREAK;
			LOG("Declaration:\n" << sections[i].GetDeclaration());

			LOG_BREAK;
			LOG("Definition:\n" << sections[i].GetDefinition());
		}

		LOG_BREAK;

		#endif // LEXER_SPLITTER_LOGGING
	}
}
