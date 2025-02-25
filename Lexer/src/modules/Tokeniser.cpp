#include <Lexer/inc/Lexer.h>

#include <unordered_map>

namespace LX
{
	namespace Constexprs
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
	}

	template<typename TokenEnum>
	struct TokeniserSettings
	{
		template<typename TokenEnum>
		class OperatorMap
		{
			private:
				const std::unordered_map<char, TokenEnum> multiCharOperators;
				const TokenEnum defaultVal;

			public:
				OperatorMap(TokenEnum val, std::unordered_map<char, TokenEnum> map)
					: defaultVal(val), multiCharOperators(map) {}

				TokenEnum GetWithNextChar(char next)
				{
					if (auto val = multiCharOperators.find(next); val != multiCharOperators.end())
					{
						return val->second;
					}

					else
					{
						return defaultVal;
					}
				}
		};

		std::unordered_map<std::string, TokenEnum> keywords;
		std::unordered_map<char, typename TokeniserSettings::OperatorMap<TokenEnum>> operators;

		TokeniserSettings(std::unordered_map<std::string, TokenEnum> keywordMap, std::unordered_map<char, OperatorMap<TokenEnum>> operatorMap)
			: keywords(keywordMap), operators(operatorMap) {}
	};

	template<typename TokenEnum, typename Token>
	static void TokeniseSource (std::vector<Token>&vec, TokeniserSettings<TokenEnum>& settings, const std::string_view& source)
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
			bool alpha = Constexprs::IsAlpha(current);

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
				if (auto keyword = settings.keywords.find(std::string(word)); keyword != settings.keywords.end())
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
				if (auto type = settings.operators.find(current); type != settings.operators.end())
				{
					char next;

					if (index + 1 > source.length())
					{
						next = ' ';
					}

					else
					{
						next = source[index + 1];
					}

					VEC_EMPLACE(vec, type->second.GetWithNextChar(next));
				}

				// Checks for whitespace and if there is none throws an error due to an unkown character
				else if (Constexprs::IsWhiteSpace(current) == false)
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

	static SectionType TokeniseDeclaration(std::vector<DecToken>& tokens, const std::string_view& source)
	{
		static TokeniserSettings<TokenTypes::Dec> settings =
		{
			// Keywords //
			{
				{ "func",		 TokenTypes::Dec::FUNCTION		}
			},

			// Operators //
			{
				{ '(', { TokenTypes::Dec::CLSE_BRACKET, {} }},
				{ ')', { TokenTypes::Dec::CLSE_BRACKET, {} }},
				{ '<', { TokenTypes::Dec::OPEN_CROCK,   {} }},
				{ '>', { TokenTypes::Dec::CLSE_CROCK,	{} }},
				{ ',', { TokenTypes::Dec::COMMA,		{} }}
			}
		};
		

		TokeniseSource<TokenTypes::Dec, DecToken>(tokens, settings, source);

		return SectionType::LX_FUNCTION;
	}

	static void TokeniseFunctionDefinition(TokenSection<FuncToken>& tokens, const std::string_view& source)
	{
		static TokeniserSettings<TokenTypes::Func> settings =
		{
			// Keywords //
			{
				{ "const",		 TokenTypes::Func::CONSTANT		},
				{ "ref",		 TokenTypes::Func::REFERENCE	},
				{ "ptr",		 TokenTypes::Func::POINTER		},
				{ "if",			 TokenTypes::Func::IF			},
				{ "elif",		 TokenTypes::Func::ELSE_IF		},
				{ "else",		 TokenTypes::Func::ELSE			},
				{ "while",		 TokenTypes::Func::WHILE		},
				{ "for",		 TokenTypes::Func::FOR			},
				{ "break",		 TokenTypes::Func::BREAK		},
				{ "continue",	 TokenTypes::Func::CONTINUE		},
				{ "return",		 TokenTypes::Func::RETURN		},
				{ "equal",		 TokenTypes::Func::EQUAL		},
				{ "and",		 TokenTypes::Func::AND			},
				{ "or",			 TokenTypes::Func::OR			},
				{ "not",		 TokenTypes::Func::NOT			}
			},

			// Operators //
			{
				// Multi-char operators //

				{
					'+',
					{
						TokenTypes::Func::ADD,
						{
							{ '+', TokenTypes::Func::INCREMENT },
							{ '=', TokenTypes::Func::C_ADD }
						}
					}
				},
				
				{
					'-',
					{
						TokenTypes::Func::SUB,
						{
							{ '-', TokenTypes::Func::DECREMENT },
							{ '=', TokenTypes::Func::C_SUB },
							{ '>', TokenTypes::Func::ARROW }
						}
					}
				},
				
				{
					'*',
					{
						TokenTypes::Func::MUL,
						{
							{ '=', TokenTypes::Func::C_MUL }
						}
					}
				},

				{
					'/',
					{
						TokenTypes::Func::DIV,
						{
							{ '=', TokenTypes::Func::C_DIV }
						}
					}
				},

				{
					'=',
					{
						TokenTypes::Func::ASSIGN,
						{
							{ '=', TokenTypes::Func::EQUAL },
							{ '>', TokenTypes::Func::DOUBLE_ARROW }
						}
					}
				},

				{
					'!',
					{
						TokenTypes::Func::NOT,
						{
							{ '=', TokenTypes::Func::N_EQUAL }
						}
					}
				},

				{
					'>',
					{
						TokenTypes::Func::GREATER_THAN,
						{
							{ '=', TokenTypes::Func::GREATER_THAN_EQUALS }
						}
					}
				},

				{
					'<',
					{
						TokenTypes::Func::LESS_THAN,
						{
							{ '=', TokenTypes::Func::LESS_THAN_EQUALS }
						}
					}
				},

				{
					':',
					{
						TokenTypes::Func::COLON,
						{
							{ ':', TokenTypes::Func::DOUBLE_COLON }
						}
					}
				},

				{
					'%',
					{
						TokenTypes::Func::MOD,
						{
							{ '=', TokenTypes::Func::C_MOD }
						}
					}
				},

				// Single char operators //

				{ '(', { TokenTypes::Func::OPEN_PAREN,  {} }},
				{ ')', { TokenTypes::Func::CLOSE_PAREN, {} }},
				{ '[', { TokenTypes::Func::OPEN_BRACK,  {} }},
				{ ']', { TokenTypes::Func::CLOSE_BRACK, {} }},
				{ '{', { TokenTypes::Func::OPEN_BRACE,  {} }},
				{ '}', { TokenTypes::Func::CLOSE_BRACE, {} }},

				{ ';', { TokenTypes::Func::SEMI_COLON,  {} }},
				{ ',', { TokenTypes::Func::COMMA,		{} }},
				{ '.', { TokenTypes::Func::DOT,         {} }}
			}
		};

		TokeniseSource<TokenTypes::Func, FuncToken>(tokens.ContentsToken(), settings, source);
	}

	void Lexer::Tokenise(std::vector<SourceSection>& sections, TokenOutput& tokens)
	{
		// Loops over the sections
		for (const auto& section : sections)
		{
			// Tokenises the declaration
			std::vector<DecToken> declaration;
			SectionType type = TokeniseDeclaration(declaration, section.GetDeclaration());

			// Finds and calls the correct function for the body of the section
			switch (type)
			{
				case SectionType::LX_FUNCTION:
				{
					TokenSection<FuncToken>& tokenSect = tokens.funcTokens.emplace_back();
					tokenSect.m_DecTokens = declaration;

					TokeniseFunctionDefinition(tokenSect, section.GetDefinition());
					break;
				}

				default:
					throw std::runtime_error("Something went horribly wrong if you are seeing this");
			}
		}
	}
}
