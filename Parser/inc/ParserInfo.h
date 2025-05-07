#pragma once

#include <LX-Common.h>

// Includes Lexer so it can use LX::Token //
#include <Lexer.h>

namespace LX
{
	// Local struct so everything can be public //
	struct ParserInfo
	{
		// Passes constructor args to members //
		ParserInfo(const std::vector<Token>& _tokens, const std::filesystem::path& path)
			: tokens(_tokens), index(0), len(_tokens.size()), scopeDepth(0), file(path)
		{}

		// The file that the tokens were generated from //
		const std::filesystem::path file;

		// Tokens created by the lexer //
		const std::vector<Token>& tokens;

		// Length of the the token vector //
		const size_t len;

		// Current index within the token vector //
		size_t index;

		// Current scope depth //
		size_t scopeDepth;
	};
}
