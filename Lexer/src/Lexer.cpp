#include <Lexer.h>
#include <Util.h>

#include <unordered_map>
#include <string_view>
#include <fstream>
#include <vector>
#include <string>

#include <iostream>
#include <iomanip>

namespace LX
{
	// Helper macro for outputting token type //
	#define TOKEN_CASE(type) case type: return #type;

	// Helper util function to translate a tokentype to it's enum val //
	static std::string ToStringNoFormat(Token::TokenType type)
	{
		switch (type)
		{
			TOKEN_CASE(Token::STRING_LITERAL);
			TOKEN_CASE(Token::IDENTIFIER);
			TOKEN_CASE(Token::FOR);
			TOKEN_CASE(Token::WHILE);
			TOKEN_CASE(Token::IF);
			TOKEN_CASE(Token::ELSE);
			TOKEN_CASE(Token::ELIF);
			TOKEN_CASE(Token::FUNCTION);
			TOKEN_CASE(Token::ADD);
			TOKEN_CASE(Token::SUB);
			TOKEN_CASE(Token::MUL);
			TOKEN_CASE(Token::DIV);
			TOKEN_CASE(Token::NUMBER_LITERAL);
			TOKEN_CASE(Token::RETURN);
			TOKEN_CASE(Token::OPEN_BRACE);
			TOKEN_CASE(Token::CLOSE_BRACE);
			TOKEN_CASE(Token::OPEN_BRACKET);
			TOKEN_CASE(Token::CLOSE_BRACKET);
			TOKEN_CASE(Token::OPEN_PAREN);
			TOKEN_CASE(Token::CLOSE_PAREN);

			default:
				return "Unknown: " + std::to_string(type);
		}
	}

	// Logging function to turn a tokentype enum val into a nicely formatted string //
	std::string ToString(Token::TokenType type)
	{
		// Gets the unformated version of the string //
		std::string unformatted = ToStringNoFormat(type);
		unformatted = unformatted.substr(7); // Removes the Token:: prefix

		// Formats the string (turns to lowercase and replaces _ with a space //
		std::string formatted;

		for (char current : unformatted)
		{
			// Adding 32 makes it lowercase due to how ASCII works //
			if ((current >= 'A' && current <= 'Z')) { formatted.push_back(current + 32); }

			// Replaces _ with spaces //
			else if (current == '_') { formatted.push_back(' '); }

			// Else adds the current character //
			else { formatted.push_back(current); }
		}

		// Returns the formatted string //
		return formatted;
	}
	
	// Stops use outside of the function //
	#undef TOKEN_CASE

	// Helper function for dealing with floating-point number literals //
	static constexpr bool CanBePartOfNumberLiteral(const char c) { return (c == '.') || (c == 'f'); }

	// Helper function to stop printing whitespace as pure whitespace //
	static std::string PrintChar(const char c)
	{
		switch (c)
		{
			// Stores them as pure string literals //
			case '\n': return R"(\n)";
			case '\t': return R"(\t)";
			case '\r': return R"(\r)";

			// Else returns a string of length one with the char inside //
			default: return std::string(1, c);
		}
	}

	// All the keywords the lexer currently supports with their token-enum equivalents //
	static const std::unordered_map<std::string, Token::TokenType> keywords =
	{
		{ "for"			, Token::FOR		},
		{ "while"		, Token::WHILE		},
		{ "if"			, Token::IF			},
		{ "else"		, Token::ELSE		},
		{ "elif"		, Token::ELIF		},
		{ "func"		, Token::FUNCTION	},
		{ "return"		, Token::RETURN		}
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
		{ ',', Token::COMMA				}
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

	// Checks if the given word is a keyword before adding it to the tokens //
	static void TokenizeWord(const std::string& word, std::vector<Token>& tokens, LexerInfo& info)
	{
		// Checks the map for a check and if so adds it with its enum equivalent //
		if (auto keyword = keywords.find(word); keyword != keywords.end())
		{
			tokens.push_back({ keyword->second, info, (std::streamsize)word.size() });
		}

		// Else adds it as a type of IDENTIFIER //
		else
		{
			tokens.push_back({ Token::IDENTIFIER, info, (std::streamsize)word.size() });
		}
	}

	const std::vector<Token> LX::LexicalAnalyze(const std::string& contents, std::streamsize len, std::ofstream* log)
	{
		// Logs the start of the lexical analysis
		SafeLog(log, LOG_BREAK, "Started lexing file", LOG_BREAK);

		// Allocates a large ammount of memory to hold the output //
		// Will shrink the size later on to stop excess memory being allocated //
		std::vector<Token> tokens = {};
		tokens.reserve(0xFFFF);

		// Trackers for when the program is iterating over the file //
		LexerInfo info;
		
		// Iterates over the file and turns it into tokens //
		while (info.index < len)
		{
			// Stores the current character for easy access
			const char current = contents[info.index];

			// Checks if it is not at end //
			// Predicts it is not at end for microptimsation //
			if (info.index + 1 < len) [[likely]]
			{
				// Gets the next character //
				const char next = contents[info.index + 1];

				// Sets flags depending on the value of the next character //
				info.isNextCharAlpha = (next >= 'a' && next <= 'z') || (next >= 'A' && next <= 'Z');
				info.isNextCharNumeric = (next >= '0' && next <= '9') || CanBePartOfNumberLiteral(next);
			}

			else
			{
				// Else defaults the next character's flags to false //
				info.isNextCharAlpha = false;
				info.isNextCharNumeric = false;
			}

			// Works out if the current character is alphabetic or numeric //
			info.isAlpha = (current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z');
			info.isNumeric = (current >= '0' && current <= '9');

			// Updates string literal tracker and skips over rest if in a string literal //
			if (current == '"')
			{
				// Start of string literal //
				if (info.inStringLiteral == false)
				{
					// Updates the neccesarry trackers //
					info.startOfStringLiteral = info.index + 1;
					info.inStringLiteral = true;
				}

				// End of string literal //
				else
				{
					// Adds the string literal token to the token vector //
					std::string lit(contents.data() + info.startOfStringLiteral, info.index - info.startOfStringLiteral);
					tokens.push_back({ Token::STRING_LITERAL, info, (std::streamsize)lit.length() + 2 }); // Adding two makes the "" be stored as well

					// Updates trackers //
					info.inStringLiteral = false;
				}
			}

			// Skips over rest if within a string literal //
			else if (info.inStringLiteral);

			// Updates comment state //
			else if (current == '#')
			{
				info.inComment = !info.inComment;
			}

			// Skips over if within a comment //
			else if (info.inComment);

			// Start of a number //
			else if (info.isNumeric == true && info.wasLastCharNumeric == false && info.lexingNumber == false)
			{
				// Stores the start of the number //
				info.startOfNumberLiteral = info.index;

				// Checks if it as the end (single char numbers) //
				if (info.isNextCharNumeric == false)
				{
					// Pushes the number to the token vector. Number literals are stored as string in the tokens //
					std::string num(contents.data() + info.startOfNumberLiteral, (unsigned __int64)(info.index + 1) - info.startOfNumberLiteral);
					tokens.push_back({ Token::NUMBER_LITERAL, info, (std::streamsize)num.size() });
				}

				// Stores it is lexing a number literal //
				else { info.lexingNumber = true; }
			}

			// End of a number //
			else if ((info.isNumeric == true || CanBePartOfNumberLiteral(current)) && info.isNextCharNumeric == false && info.lexingNumber == true)
			{
				// Pushes the number to the token vector. Number literals are stored as string in the tokens //
				std::string num(contents.data() + info.startOfNumberLiteral, (unsigned __int64)(info.index + 1) - info.startOfNumberLiteral);
				tokens.push_back({ Token::NUMBER_LITERAL, info, (std::streamsize)num.size() });
				info.lexingNumber = false; // Stops storing it is lexing a number
			}

			// During a number //
			else if (info.isNumeric == true);
			else if (info.lexingNumber == true && CanBePartOfNumberLiteral(current));

			// Start of a word //
			else if (info.isAlpha == true && info.wasLastCharAlpha == false)
			{
				// Stores the start of the word //
				info.startOfWord = info.index;

				// Checks if it is at the end (single char words) //
				if (info.isNextCharAlpha == false)
				{
					// Calls the function designed to handle the tokenisation of words //
					TokenizeWord({ contents.data() + info.startOfWord, 1 }, tokens, info);
				}
			}

			// End of a word //
			else if (info.isAlpha == true && info.isNextCharAlpha == false)
			{
				// Calls the function designed to handle the tokenisation of words //
				TokenizeWord({ contents.data() + info.startOfWord, (unsigned __int64)((info.index + 1) - info.startOfWord) }, tokens, info);
			}

			// During a word //
			else if (info.isAlpha == true);

			// Symbols //
			else if (auto sym = symbols.find(current); sym != symbols.end())
			{
				tokens.push_back({ sym->second, info, 1 });
			}

			// Operators (+, -, /, *) //
			else if (auto op = operators.find(current); op != operators.end())
			{
				tokens.push_back({ op->second, info, 1 });
			}

			// If it is here and not whitespace that means it's an invalid character //
			else if (current == ' ' || current == '\r');

			// Skips over an extra 3 spaces as tabs SHOULD ALWAYS take up 4 spaces //
			// Only for the column and not index //
			else if (current == '\t')
			{
				info.column = info.column + 3;
			}

			// Increments the line number and resets the column on entering a new line //
			else if (current == '\n')
			{
				info.column = 0;
				info.line++;
			}

			// Throws an error with all the relevant information //
			else
			{
				throw InvalidCharInSource(info.column, info.line, info.index, contents[info.index]);
			}

			// Log dumps A LOT of info //

			#ifdef LOG_EVERYTHING

			SafeLog
			(
				log, "Index: ", std::left, std::setw(3), info.index,
				" Is Alpha: ", info.isAlpha,
				" Is Numeric: ", info.isNumeric,
				" In Comment: ", info.inComment,
				" In String: ", info.inStringLiteral,
				" Next Char Alpha: ", info.isNextCharAlpha,
				" Next Char Numeric: ", info.wasLastCharNumeric,
				" Last Char Numeric: ", info.wasLastCharAlpha,
				" Lexing number: ", info.lexingNumber,
				" Current: {", PrintChar(current), "}"
			);

			#endif // LOG_EVERYTHING

			// Updates trackers to their default state of a new character //

			info.index++;
			info.column++;

			info.wasLastCharAlpha = info.isAlpha;
			info.wasLastCharNumeric = info.isNumeric;
		}

		// Logs the tokens if logging is on //
		if (log != nullptr)
		{
			#ifdef LOG_EVERYTHING
			SafeLog(log, "\n"); // Puts a space when there is a lot in the log
			#endif // LOG_EVERYTHING

			for (auto& token : tokens)
			{
				SafeLog
				(
					log, std::left, 
					"{ Line: ", std::setw(3), token.line, 
					", Index: ", std::setw(3), token.index,
					", Length: ", std::setw(2), token.length, " } ",
					std::setw(30), ToStringNoFormat(token.type) + ":", "{", token.GetContents(), "}"
				);
			}

			SafeLog(log, "\n END OF TOKENS");
		}

		// Shrinks the vector down to minimum size before returning to avoid excess memory being allocated
		tokens.shrink_to_fit();
		return tokens;
	}
}