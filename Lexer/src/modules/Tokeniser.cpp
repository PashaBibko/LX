#include <Lexer/inc/Lexer.h>

namespace LX
{
	struct TokeniseDeclarationReturnType
	{
		std::vector<DecToken> tokens;
		SectionType type;
	};

	static TokeniseDeclarationReturnType TokeniseDeclaration(const std::string_view& source)
	{
		TokeniseDeclarationReturnType out;

		out.type = SectionType::LX_FUNCTION;

		return out;
	}

	static EmptyTokenSection TokeniseFunctionDefinition(const std::string_view& source)
	{
		// Union to allow for easier changing between EmptyTokenSection and it's true type
		union Tokens
		{
			public:
				TokenSection<FuncToken> interactable;
				EmptyTokenSection output;

				Tokens() { new (&output) TokenSection<FuncToken>(); }
				~Tokens() {}
		};
		
		// Stores the tokens
		Tokens tokens;

		// Returns the tokens in their output type
		return tokens.output;
	}

	std::vector<EmptyTokenSection> Lexer::Tokenise(std::vector<SourceSection>& sections)
	{
		// Creates the output vector and reserves all the space that should be needed
		std::vector<EmptyTokenSection> tokenSections;
		tokenSections.reserve(sections.size());

		for (const auto& section : sections)
		{
			// Tokenises the declaration
			auto [declaration, type] = TokeniseDeclaration(section.GetDeclaration());

			// Finds and calls the correct function for the body of the section
			switch (type)
			{
				case SectionType::LX_FUNCTION:
					tokenSections.push_back(TokeniseFunctionDefinition(section.GetDefinition()));
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
