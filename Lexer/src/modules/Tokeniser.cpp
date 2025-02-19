#include <Lexer/inc/Lexer.h>

namespace LX
{
	// Union to allow for easier changing between EmptyTokenSection and it's true type
	union TokenVector
	{
		public:
			// Default type
			EmptyTokenSection* empty;

			// Types passed to functions

			TokenSection<FuncToken>* func;

			// Constructor to make sure the pointer is not null
			TokenVector(EmptyTokenSection& ref)
				: empty(&ref)
			{}
	};

	static SectionType TokeniseDeclaration(TokenVector vec, const std::string_view& source)
	{
		// Index within the string_view
		size_t index = 0;

		while (index < source.length())
		{
			// Gets the current char
			const char current = source[index];

			switch (current)
			{
				case '(':
					VEC_EMPLACE(vec.empty->DecTokens(), TokenTypes::Dec::OPEN_BRACKET);
					break;

				case ')':
					VEC_EMPLACE(vec.empty->DecTokens(), TokenTypes::Dec::CLSE_BRACKET);
					break;

				case '<':
					VEC_EMPLACE(vec.empty->DecTokens(), TokenTypes::Dec::OPEN_CROCK);
					break;

				case '>':
					VEC_EMPLACE(vec.empty->DecTokens(), TokenTypes::Dec::CLSE_CROCK);
					break;
			}

			index++; // Iterates to the next character
		}

		return SectionType::LX_FUNCTION;
	}

	static void TokeniseFunctionDefinition(TokenVector vec, const std::string_view& source)
	{
		vec.func->ContentsToken().push_back(TokenTypes::Func::IDENTIFIER);
	}

	std::vector<EmptyTokenSection> Lexer::Tokenise(std::vector<SourceSection>& sections)
	{
		// Creates the output vector and reserves all the space that should be needed
		std::vector<EmptyTokenSection> tokenSections;
		tokenSections.reserve(sections.size());

		for (const auto& section : sections)
		{
			// Creates a new object in the vector and creates a reference to it
			tokenSections.push_back(EmptyTokenSection());
			TokenVector tokens(tokenSections.back());

			// Tokenises the declaration
			SectionType type = TokeniseDeclaration(tokens, section.GetDeclaration());

			// Finds and calls the correct function for the body of the section
			switch (type)
			{
				case SectionType::LX_FUNCTION:
					TokeniseFunctionDefinition(tokens, section.GetDefinition());
					break;

				default:
					throw std::runtime_error("Something went horribly wrong if you are seeing this");
			}
		}

		// Shrinks vector to avoid more memory being allocated than needed before returning
		tokenSections.shrink_to_fit();
		return tokenSections;
	}
}
