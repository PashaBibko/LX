#pragma once

#include <LX-Common.h>

namespace LX
{
	// Struct to store the current information of the lexer //
	struct LexerInfo
	{
		// Constructor to set the constants //
		LexerInfo(const std::string& _source)
			: source(_source), len(_source.length())
		{}

		// Current trackers of where in the source it is //

		std::streamsize line = 1; // <- Lines start on 1 (probably because of non-programmer's)
		std::streamsize index = 0;
		std::streamsize column = 0; // <- Columns start on 1 (probably because of non-programmer's). THEN WHY IS THIS SET TO 0

		// Trackers for when a multi-char token started //

		std::streamsize startOfWord = 0;
		std::streamsize startOfNumberLiteral = 0;
		std::streamsize startOfStringLiteral = 0;

		// Information about the source //

		const std::string& source;
		const std::streamsize len;

		// Different flags of the lexer //
		// Stored as a bitset to minimse memory allocated //
		// - Basically no difference, because only one exists at any given time //
		// - But it is a cool C++ feature I like so I use it //

		bool isAlpha : 1 = false;
		bool isNumeric : 1 = false;
		bool inComment : 1 = false;
		bool inStringLiteral : 1 = false;
		bool isNextCharAlpha : 1 = false;
		bool isNextCharNumeric : 1 = false;
		bool wasLastCharAlpha : 1 = false;
		bool wasLastCharNumeric : 1 = false;
		bool lexingNumber : 1 = false;
	};
}
