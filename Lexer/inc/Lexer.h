#pragma once

#include <Lexer/inc/Section.h>
#include <Lexer/inc/Token.h>

#include <Common/Macro.h>

#include <vector>

namespace LX
{
	std::string ReadFile(const std::string& filename);

	struct SplitterInfo
	{
		// Current sections outputted by the splitter
		std::vector<SourceSection> sections;

		// Current index in the source
		size_t index = 0;

		// Current depth of scopes
		unsigned short depth = 0;

		// Tracks wether it is in a comment
		bool inComment = false;

		// Start of the current block
		size_t blockStart = 0;

		// End of the last block
		size_t endOfLastBlock = 0;
	};

	struct TokenOutput
	{
		std::vector<TokenSection<FuncToken>> funcTokens;
	};

	class Lexer
	{
		private:
			// Makes other parts of the code look less cursed
			using SplitterHelper = void(Lexer::*)(SplitterInfo&);

			// Allows this function to access members within this class
			friend constexpr Lexer::SplitterHelper GetOperationImpl(char val) noexcept;

			// Current source
			const std::string& m_Source;

			// Splitter helper functions

			void OnBrceOpn(SplitterInfo& info);
			void OnBrceCls(SplitterInfo& info);
			void OnHashtag(SplitterInfo& info);

			// Splits the source into smaller sections
			std::vector<SourceSection> Split();

			//
			void Tokenise(std::vector<SourceSection>& sections, TokenOutput& tokens);

		public:
			Lexer(const std::string& src, TokenOutput& tokens);
	};
}
