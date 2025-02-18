#pragma once

#include <Lexer/inc/Section.h>
#include <Lexer/inc/Token.h>

#include <Common/Macro.h>

#include <vector>

namespace LX
{
	std::string ReadFile(const std::string& filename);

	class Lexer
	{
		private:
			HEADER("Misc stuff");

			// Makes other parts of the code look less cursed
			using LexerFun = void(Lexer::*)();

			// Allows this function to access members within this class
			friend constexpr Lexer::LexerFun GetOperationImpl(char val) noexcept;

			HEADER("Info about the source");

			// The source it will be running the lexical anylisis on
			const std::string& m_Source;

			// Length of the current source
			const size_t m_SourceLen;

			HEADER("Info about index within source");

			// Current index in the source
			size_t m_Index = 0;

			// What line the splitter is on
			size_t m_LineIndex = 1;

			// Index of the line it is currently on
			short m_IndexOnLine = 1;

			// Current depth of scopes
			unsigned short m_Depth = 0;

			HEADER("Flags");

			bool m_InComment = false;

			HEADER("Info about current sections within source");

			// Start of the current block
			size_t m_BlockStart = 0;

			// End of the last block
			size_t m_EndOfLastBlock = 0;

			// Current output
			std::vector<SourceSection> m_Sections;

			void OnNewLine();
			void OnBrceOpn();
			void OnBrceCls();
			void OnHashtag();

			void Split();

		public:
			Lexer(const std::string& src);
	};
}