#include <Lexer/inc/Lexer.h>

namespace LX
{
	static constexpr bool IsAlpha(char c)
	{
		return (c >= 'a' && c <= 'z') ||
			   (c >= 'A' && c <= 'Z') ||
			   (c >= '0' && c <= '9');
	}

	static constexpr bool IsWhiteSpace(char c)
	{
		return c == '\t' ||
			   c == '\n' ||
			   c == '\r' ||
			   c == ' ';
	}


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
				: empty(&ref) {}
	};

	static SectionType TokeniseDeclaration(TokenVector vec, const std::string_view& source)
	{
		// Index within the string_view
		size_t index = 0;

		// Tracks where the current word starts
		size_t startOfWord = 0;

		// Tracks wether the last character was part of the alphabet
		bool wasLastCharAlpha = false;

		// Tracks if the source is currently in a comment
		bool inComment = false;

		// Loops over the string view to tokenise it
		while (index < source.length())
		{
			// Gets the current char
			const char current = source[index];

			// Updates comment tracker
			if (current == '#')
			{
				inComment = !inComment;

				index++;
				continue;
			}

			// Skips over the rest of the logic if within a comment
			if (inComment)
			{
				index++;
				continue;
			}

			// Tracks if the current character is part of the alphabet
			bool currentlyAlpha = IsAlpha(current);

			// Looks for special characters
			if (currentlyAlpha == false)
			{
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

					case ',':
						VEC_EMPLACE(vec.empty->DecTokens(), TokenTypes::Dec::COMMA);
						break;

					default:
						// Avoids whitespace from flagging any accidental errors
						if (IsWhiteSpace(current) == false)
						{
							LOG("UNKNOWN CHARACTER: " << current);
						}
						break;
				}
			}

			// Stores the index of the beginning of the word if needed
			if (currentlyAlpha == true && wasLastCharAlpha == false)
			{
				startOfWord = index;
			}

			// Adds a token if at the end of the word
			else if (currentlyAlpha == false && wasLastCharAlpha == true)
			{
				std::string_view word(source.data() + startOfWord, index - startOfWord);

				VEC_EMPLACE(vec.empty->DecTokens(), TokenTypes::Dec::IDENTIFIER, word);
			}

			// Updates tracker of last character state
			wasLastCharAlpha = currentlyAlpha;

			// Iterates to the next character
			index++;
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
