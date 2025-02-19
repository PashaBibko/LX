#pragma once

#include <Common/Poly.h>

#include <Lexer/inc/Token.h>

#include <string_view>
#include <vector>

namespace LX
{
	class Lexer;

	enum class SectionType
	{
		LX_FUNCTION,

		UNDEFINED = 0
	};

	class SourceSection final
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
	
	template<typename TokenType>
	class TokenSection : public PolyBase
		<SectionType, SectionType::UNDEFINED>
	{
		private:
			// Friends the lexer to allow it to add to the vectors
			friend class Lexer;

			// Contents of the section
			std::vector<TokenType> m_Contents;

			// Declaration of the section
			std::vector<DecToken> m_DecTokens;

		public:
			TokenSection()
			{
				// Reserves memory to avoid excess allocations later
				m_DecTokens.reserve(16);
				m_Contents.reserve(16);
			}

			void RemoveExcess()
			{
				// Shrinks all the vectors to reduce memory footprint
				m_DecTokens.shrink_to_fit();
				m_Contents.shrink_to_fit();
			}

			// Getter for the contents vector
			inline TokenType& ContentsAt(size_t index) { return m_Contents[index]; }

			// Getter for the declaration vector
			inline DecToken& DecAt(size_t index) { return m_DecTokens[index]; }

			// Getter to allow pushing to the declaration
			inline std::vector<DecToken>& DecTokens() { return m_DecTokens; }

			// Getter to allow pushing to the contents
			inline std::vector<TokenType>& ContentsToken() { return m_Contents; }
	};

	// Typedef used for polymorphism
	typedef TokenSection<TokenBase<short, -1>> EmptyTokenSection;
}
