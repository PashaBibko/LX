#include <Lexer/inc/Lexer.h>

namespace LX
{
	constexpr Lexer::SplitterHelper GetOperationImpl(char val) noexcept
	{
		return (val == '{') ? &Lexer::OnBrceOpn :
			   (val == '}') ? &Lexer::OnBrceCls :
			   (val == '#') ? &Lexer::OnHashtag :
			   nullptr;
	}

	void Lexer::OnBrceOpn(SplitterInfo& info)
	{
		// Comments nullify this function so it early returns
		if (info.inComment) { return; }

		// Starts a block if at the top level
		if (info.depth++ == 0) { info.blockStart = info.index; }

		#ifdef LEXER_SPLITTER_LOGGING

		// Logs the char being found
		LOG("Found brace open at: " << info.index);

		#endif // LEXER_SPLITTER_LOGGING
	}

	void Lexer::OnBrceCls(SplitterInfo& info)
	{
		// Comments nullify this function so it early returns
		if (info.inComment) { return; }

		// Adds a source section if in a bottom source block
		if (info.depth-- == 1)
		{
			// Adds the new block to the vector of sections
			// Uses a macro because I prefer std::vector.push_back() errors
			VEC_EMPLACE
			(
				// The vector to push/emplace to
				info.sections,

				// The block is given a reference to its creator
				*this,

				// String view of the declaration
				std::string_view(m_Source.data() + info.endOfLastBlock, info.blockStart - info.endOfLastBlock),

				// String view of the definition
				std::string_view(m_Source.data() + info.blockStart + 1, info.index - info.blockStart - 1)
			);

			// Sets end of last block to current index for the next creation
			info.endOfLastBlock = info.index + 1;
		}

		#ifdef LEXER_SPLITTER_LOGGING

		// Logs the char being found
		LOG("Found brace close at: " << info.index);

		#endif // LEXER_SPLITTER_LOGGING
	}

	void Lexer::OnHashtag(SplitterInfo& info)
	{
		// Toggles comment flag
		info.inComment = !info.inComment;

		#ifdef LEXER_SPLITTER_LOGGING

		// Logs the char being found
		LOG("Found hashtag at: " << info.index);

		#endif // LEXER_SPLITTER_LOGGING
	}

	std::vector<SourceSection> Lexer::Split()
	{
		// Current info of the splitter
		SplitterInfo info;
		info.sections.reserve(8); // Reserves a small ammount to reduce uneccesarry allocations

		// Gets the length and stores it to avoid repeat function calls
		size_t len = m_Source.length();

		// Loops over the source to split it up
		while (info.index < len)
		{
			// If valid, calls the function
			if (SplitterHelper func = GetOperationImpl(m_Source[info.index]))
			{
				(this->*func)(info);
			}

			// Iterates
			info.index++;
		}

		info.sections.shrink_to_fit();

		return info.sections;
	}
}
