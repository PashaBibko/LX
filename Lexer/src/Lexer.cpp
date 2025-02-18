#include <Lexer/inc/Lexer.h>

#include <stdexcept>

constexpr Lexer::LexerFun GetOperationImpl(char val) noexcept
{
	return (val == '\n') ? &Lexer::OnNewLine :
		   (val == '{' ) ? &Lexer::OnBrceOpn :
		   (val == '}' ) ? &Lexer::OnBrceCls :
		   (val == '#' ) ? &Lexer::OnHashtag :
		   nullptr;
}

enum class LexFlags : short
{
	COMMENT = 0b01,
	BCKSLSH = 0b10,

	NONE = 0x0
};

void Lexer::OnNewLine()
{
	// Resets line relative counters
	m_LineIndex++;
	m_IndexOnLine = 0;
}

void Lexer::OnBrceOpn()
{
	// Comments nullify this function so it early returns
	if (m_InComment) { return; }

	// Starts a block if at the top level
	if (m_Depth++ == 0) { m_BlockStart = m_Index; }

	#ifdef LEXER_SPLITTER_LOGGING

	// Logs the char being found
	LOG("Found brace open at: " << m_LineIndex << " : " << m_IndexOnLine);

	#endif // LEXER_SPLITTER_LOGGING
}

void Lexer::OnBrceCls()
{
	// Comments nullify this function so it early returns
	if (m_InComment) { return; }

	// Adds a source section if in a bottom source block
	if (m_Depth-- == 1)
	{
		// Adds the new block to the vector of sections
		// Uses a macro because I prefer std::vector.push_back() errors
		VEC_EMPLACE
		(
			// The vector to push/emplace to
			m_Sections,

			// The block is given a reference to its creator
			*this,

			// String view of the declaration
			std::string_view(m_Source.data() + m_EndOfLastBlock, m_BlockStart - m_EndOfLastBlock),

			// String view of the definition
			std::string_view(m_Source.data() + m_BlockStart + 1, m_Index - m_BlockStart - 1)
		);

		// Sets end of last block to current index for the next creation
		m_EndOfLastBlock = m_Index + 1;
	}

	#ifdef LEXER_SPLITTER_LOGGING

	// Logs the char being found
	LOG("Found brace close at: " << m_LineIndex << " : " << m_IndexOnLine);

	#endif // LEXER_SPLITTER_LOGGING
}

void Lexer::OnHashtag()
{
	// Toggles comment flag
	m_InComment = !m_InComment;

	#ifdef LEXER_SPLITTER_LOGGING

	// Logs the char being found
	LOG("Found hashtag at: " << m_LineIndex << " : " << m_IndexOnLine);

	#endif // LEXER_SPLITTER_LOGGING
}

void Lexer::Split()
{
	while (m_Index < m_SourceLen)
	{
		// If valid, calls the function
		if (LexerFun func = GetOperationImpl(m_Source[m_Index]))
		{
			(this->*func)();
		}

		// Iterates
		m_Index++;
		m_IndexOnLine++;
	}
}

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
