#include <Lexer/inc/Lexer.h>

#include <stdexcept>

#define CASE(token) case token: os << #token; return os

namespace LX
{
	static std::ostream& operator<<(std::ostream& os, TokenTypes::Dec val)
	{
		using namespace TokenTypes;

		switch (val)
		{
			CASE(Dec::OPEN_BRACKET);
			CASE(Dec::CLSE_BRACKET);
			CASE(Dec::OPEN_CROCK);
			CASE(Dec::CLSE_CROCK);
			CASE(Dec::IDENTIFIER);
			CASE(Dec::COMMA);
			CASE(Dec::UNDEFINED);

			default:
				os << "Dec<error>(" << (short)val << ")";
				return os;
		}
	}

	static std::ostream& operator<<(std::ostream& os, TokenTypes::Func val)
	{
		using namespace TokenTypes;

		switch (val)
		{
			CASE(Func::IDENTIFIER);
			CASE(Func::INT_DEC);
			CASE(Func::STR_DEC);
			CASE(Func::STRING_LITERAL);
			CASE(Func::NUMBER_LITERAL);
			CASE(Func::CONSTANT);
			CASE(Func::REFERENCE);
			CASE(Func::POINTER);
			CASE(Func::IF);
			CASE(Func::ELSE_IF);
			CASE(Func::ELSE);
			CASE(Func::WHILE);
			CASE(Func::FOR);
			CASE(Func::FOR_EACH);
			CASE(Func::BREAK);
			CASE(Func::CONTINUE);
			CASE(Func::RETURN);
			CASE(Func::ADD);
			CASE(Func::SUB);
			CASE(Func::MUL);
			CASE(Func::DIV);
			CASE(Func::MOD);
			CASE(Func::C_ADD);
			CASE(Func::C_SUB);
			CASE(Func::C_MUL);
			CASE(Func::C_DIV);
			CASE(Func::C_MOD);
			CASE(Func::INCREMENT);
			CASE(Func::DECREMENT);
			CASE(Func::EQUAL);
			CASE(Func::N_EQUAL);
			CASE(Func::GREATER_THAN);
			CASE(Func::GREATER_THAN_EQUALS);
			CASE(Func::LESS_THAN);
			CASE(Func::LESS_THAN_EQUALS);
			CASE(Func::AND);
			CASE(Func::OR);
			CASE(Func::NOT);
			CASE(Func::COMMA);
			CASE(Func::SEMI_COLON);
			CASE(Func::COLON);
			CASE(Func::ASSIGN);
			CASE(Func::DOT);
			CASE(Func::ARROW);
			CASE(Func::DOUBLE_ARROW);
			CASE(Func::DOUBLE_COLON);
			CASE(Func::OPEN_PAREN);
			CASE(Func::CLOSE_PAREN);
			CASE(Func::OPEN_BRACK);
			CASE(Func::CLOSE_BRACK);
			CASE(Func::OPEN_BRACE);
			CASE(Func::CLOSE_BRACE);
			CASE(Func::UNDEFINED);

			default:
				os << "Func<error>(" << (short)val << ")";
				return os;
		}
	}

	Lexer::Lexer(const std::string& src)
		: m_Source(src)
	{
		// Checks source is a valid length
		if (m_Source.length() == 0x0)
		{
			throw std::runtime_error("Source cannot be empty");
		}

		// Splits the source into sections
		std::vector<SourceSection> sections = Split();

		#ifdef LEXER_SPLITTER_LOGGING

		LOG_BREAK;
		LOG("Section Count: " << sections.size());

		// Prints the blocks to the console
		for (int i = 0; i < sections.size(); i++)
		{
			LOG_BREAK;
			LOG("Section: " << i);

			LOG_BREAK;
			LOG("Declaration:\n" << sections[i].GetDeclaration());

			LOG_BREAK;
			LOG("Definition:\n" << sections[i].GetDefinition());
		}

		LOG_BREAK;

		#endif // LEXER_SPLITTER_LOGGING

		std::vector<EmptyTokenSection> tokens = Tokenise(sections);

		#ifdef LEXER_TOKENISER_LOGGING

		LOG_BREAK;
		LOG("Token section count: " << tokens.size());

		int counter = 0;

		for (EmptyTokenSection& tokenSect : tokens)
		{
			LOG_BREAK;
			LOG("Token section: " << counter);
			counter++;

			LOG_BREAK;
			LOG("Declaration tokens");
			LOG_BREAK;

			union FunctionTokens
			{
				public:
					TokenSection<FuncToken> funcTokens;
					EmptyTokenSection emptyTokens;

					FunctionTokens() { new (&emptyTokens)EmptyTokenSection(); }
					~FunctionTokens() {}
			};

			//
			FunctionTokens funcTokens;
			funcTokens.emptyTokens = tokenSect;

			//
			for (DecToken& token : funcTokens.funcTokens.m_DecTokens)
			{
				LOG(token.m_Type << " {" << token.m_Contents << '}');
			}

			LOG_BREAK;
			LOG("Content tokens");
			LOG_BREAK;

			for (FuncToken& token : funcTokens.funcTokens.m_Contents)
			{
				LOG(token.m_Type << " {" << token.m_Contents << '}');
			}
		}

		LOG_BREAK;

		#endif // LEXER_TOKENISER_LOGGING

	}
}
