#include <LX-Common.h>

#include <Lexer.h>

namespace LX
{
	// All the keywords the lexer currently supports with their token-enum equivalents //
	static const std::unordered_map<std::string, Token::TokenType> keywords =
	{
		{ "for"			, Token::FOR		},
		{ "while"		, Token::WHILE		},
		{ "if"			, Token::IF			},
		{ "else"		, Token::ELSE		},
		{ "elif"		, Token::ELIF		},
		{ "func"		, Token::FUNCTION	},
		{ "return"		, Token::RETURN		},
		{ "int"			, Token::INT_DEC	}
	};

	// All the symbols supported by the lexer //
	static const std::unordered_map<char, Token::TokenType> symbols =
	{
		{ '{', Token::OPEN_BRACKET		},
		{ '}', Token::CLOSE_BRACKET		},
		{ '[', Token::OPEN_BRACE		},
		{ ']', Token::CLOSE_BRACE		},
		{ '(', Token::OPEN_PAREN		},
		{ ')', Token::CLOSE_PAREN		},
		{ ',', Token::COMMA				},
		{ '=', Token::ASSIGN			}
	};

	// All the single-char operators currently supported by the lexer with their token-enum equivalents //
	// TODO: Support multi-char operators such as: ==, -> +=, &&
	static const std::unordered_map<char, Token::TokenType> operators =
	{
		{ '+', Token::ADD },
		{ '-', Token::SUB },
		{ '*', Token::MUL },
		{ '/', Token::DIV }
	};
}
