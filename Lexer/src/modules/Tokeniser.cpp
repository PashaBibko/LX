#include <Lexer/inc/Lexer.h>

#include <unordered_map>

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
	
	// Temporary way to pass in the token maps
	#define TOKEN_MAP(type) const std::unordered_map<type, TokenEnum>&

	template<typename TokenEnum, typename Token>
	static void TokeniseSource (std::vector<Token>&vec, TOKEN_MAP(std::string) keywords, TOKEN_MAP(char) operators, const std::string_view& source)
	{
		// Index within the string view
		size_t index = 0;

		// Tracks where the current word starts
		size_t startOfWord = 0;

		// Tracks wether the last character was part of the alphabet
		bool wasLastCharAlpha = false;

		// Is the source currently in a comment
		bool inComment = false;

		// Loops over the string view to tokenise it
		while (index < source.length())
		{
			// The current char within the source
			const char current = source[index];

			// Updates comment tracker
			if (current == '#')
			{
				inComment = !inComment;

				// Updates index to stop errors and jumps to next iteration
				index++;
				continue;
			}

			// Skips over the rest of the logic if within a comment
			if (inComment)
			{
				index++;
				continue;
			}

			// Is currently over an alphabetic character
			bool alpha = IsAlpha(current);

			// Stores the index at the beginning of the word if needed
			if (alpha == true && wasLastCharAlpha == false)
			{
				startOfWord = index;
			}

			// If at the end of a word adds it to the token vector
			else if (alpha == false && wasLastCharAlpha == true)
			{
				// Creates a view to the word
				std::string_view word(source.data() + startOfWord, index - startOfWord);

				// If it is a keyword it adds it to the vector
				if (auto keyword = keywords.find(std::string(word)); keyword != keywords.end())
				{
					VEC_EMPLACE(vec, keyword->second);
				}

				// Else adds it as an identifier
				else
				{
					VEC_EMPLACE(vec, TokenEnum::IDENTIFIER, word);
				}
			}

			// Looks for operators if needed
			if (alpha == false)
			{
				// If its an operator the operator is added to the vector
				if (auto type = operators.find(current); type != operators.end())
				{
					VEC_EMPLACE(vec, type->second);
				}

				// Checks for whitespace and if there is none throws an error due to an unkown character
				else if (IsWhiteSpace(current) == false)
				{
					LOG("UNKNOWN CHARACTER: " << current);
				}
			}

			// Updates tracker of last character state
			wasLastCharAlpha = alpha;

			// Iterates to next character
			index++;
		}
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
				: empty(&ref) {
			}
	};

	static SectionType TokeniseDeclaration(TokenVector vec, const std::string_view& source)
	{
		static std::unordered_map<std::string, TokenTypes::Dec> keywords =
		{
			{ "func",		 TokenTypes::Dec::FUNCTION		}
		};

		static std::unordered_map<char, TokenTypes::Dec> operators =
		{
			{ '(',			 TokenTypes::Dec::OPEN_BRACKET  },
			{ ')',			 TokenTypes::Dec::CLSE_BRACKET  },
			{ '<',			 TokenTypes::Dec::OPEN_CROCK    },
			{ '>',			 TokenTypes::Dec::CLSE_CROCK	},
			{ ',',			 TokenTypes::Dec::COMMA			}
		};

		TokeniseSource<TokenTypes::Dec, DecToken>(vec.empty->DecTokens(), keywords, operators, source);

		return SectionType::LX_FUNCTION;
	}

	static void TokeniseFunctionDefinition(TokenVector vec, const std::string_view& source)
	{
		static std::unordered_map<std::string, TokenTypes::Func> keywords =
		{
			{ "const",		 TokenTypes::Func::CONSTANT		},
			{ "ref",		 TokenTypes::Func::REFERENCE	},
			{ "ptr",		 TokenTypes::Func::POINTER		},
			{ "if",			 TokenTypes::Func::IF			},
			{ "elif",		 TokenTypes::Func::ELSE_IF		},
			{ "else",		 TokenTypes::Func::ELSE			},
			{ "while",		 TokenTypes::Func::WHILE		},
			{ "for",		 TokenTypes::Func::FOR			},
			{ "foreach",	 TokenTypes::Func::FOR_EACH		},
			{ "break",		 TokenTypes::Func::BREAK		},
			{ "continue",	 TokenTypes::Func::CONTINUE		},
			{ "return",		 TokenTypes::Func::RETURN		},
			{ "equal",		 TokenTypes::Func::EQUAL		},
			{ "and",		 TokenTypes::Func::AND			},
			{ "or",			 TokenTypes::Func::OR			},
			{ "not",		 TokenTypes::Func::NOT			}
		};

		std::vector<FuncToken>& tokens = vec.func->ContentsToken();

		tokens.push_back(TokenTypes::Func::IDENTIFIER);
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
