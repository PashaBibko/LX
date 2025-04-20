#include <Lexer.h>
#include <Util.h>

#include <unordered_map>
#include <string_view>
#include <fstream>
#include <vector>

#include <iostream>

namespace LX
{
	// Local macros cause im lazy //

	#define TOKEN_CASE(type) case type: return #type;

	// Logging function to turn a tokentype enum val into it's string //
	static std::string ToString(Token::TokenType type)
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

			default:
				return "Unknown: " + std::to_string(type);
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
	static void TokenizeWord(const std::string& word, std::vector<Token>& tokens)
	{
		// Checks the map for a check and if so adds it with its enum equivalent //
		if (auto keyword = keywords.find(word); keyword != keywords.end())
		{
			tokens.push_back({ keyword->second, "" });
		}

		// Else adds it as a type of IDENTIFIER //
		else
		{
			tokens.push_back({ Token::IDENTIFIER, word });
		}
	}

	// Struct to store the current information of the lexer //
	struct LexerInfo
	{
		// Current index within the lexer //
		std::streamsize index = 0;

		// Trackers for when a multi-char token started //

		std::streamsize startOfWord = 0;
		std::streamsize startOfNumberLiteral = 0;
		std::streamsize startOfStringLiteral = 0;

		// Different flags of the lexer //
		// Stored as a bitset to minimse memory allocated (basically no difference, because only one exists at any given time) //

		bool isAlpha						: 1 = false;
		bool isNumeric						: 1 = false;
		bool inComment						: 1 = false;
		bool inStringLiteral				: 1 = false;
		bool isNextCharAlpha				: 1 = false;
		bool isNextCharNumeric				: 1 = false;
		bool wasLastCharAlpha				: 1 = false;
		bool wasLastCharNumeric				: 1 = false;
	};

	const std::vector<Token> LX::LexicalAnalyze(std::ifstream& src, std::ofstream* log)
	{
		// Logs the start of the lexical analysis
		SafeLog(log, LOG_BREAK, "Started lexing file", LOG_BREAK);

		// Allocates a large ammount of memory to hold the output //
		// Will shrink the size later on to stop excess memory being allocated //
		std::vector<Token> tokens = {};
		tokens.reserve(0xFFFF);

		// Turns the contents of the file into a string //

		// Gets length of the file because it is opened at the end
		const std::streamsize len = src.tellg();
		src.seekg(0, std::ios::beg); // Goes back to the beginning

		std::string contents(len, '\0'); // Preallocates all space needed
		src.read(&contents[0], len); // Transfers file to string

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
				info.isNextCharNumeric = (next >= '0' && next <= '9');
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
					tokens.push_back({ Token::STRING_LITERAL, lit });

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

			// Start of a word //
			else if (info.isAlpha == true && info.wasLastCharAlpha == false)
			{
				// Stores the start of the word //
				info.startOfWord = info.index;

				// Checks if it is at the end (single char words) //
				if (info.isNextCharAlpha == false)
				{
					// Calls the function designed to handle the tokenisation of words //
					TokenizeWord({ contents.data() + info.startOfWord, 1 }, tokens);
				}
			}

			// End of a word //
			else if (info.isAlpha == true && info.isNextCharAlpha == false)
			{
				// Calls the function designed to handle the tokenisation of words //
				TokenizeWord({ contents.data() + info.startOfWord, (unsigned __int64)((info.index + 1) - info.startOfWord) }, tokens);
			}

			// During a word //
			else if (info.isAlpha == true);

			// Start of a number //
			else if (info.isNumeric == true && info.wasLastCharNumeric == false)
			{
				// Stores the start of the number //
				info.startOfNumberLiteral = info.index;

				// Checks if it as the end (single char numbers) //
				if (info.isNextCharNumeric == false)
				{
					// Pushes the number to the token vector. Number literals are stored as string in the tokens //
					std::string num(contents.data() + info.startOfNumberLiteral, (unsigned __int64)(info.index + 1) - info.startOfNumberLiteral);
					tokens.push_back({ Token::NUMBER_LITERAL, num });
				}
			}

			// End of a number //
			else if (info.isNumeric == true && info.isNextCharNumeric == false)
			{
				// Pushes the number to the token vector. Number literals are stored as string in the tokens //
				std::string num(contents.data() + info.startOfNumberLiteral, (unsigned __int64)(info.index + 1) - info.startOfNumberLiteral);
				tokens.push_back({ Token::NUMBER_LITERAL, num });
			}

			// During a number //
			else if (info.isNumeric == true);

			// Operators (+, -, /, *) //
			else if (auto op = operators.find(current); op != operators.end())
			{
				tokens.push_back({ op->second, "" });
			}

			// If it is here and not whitespace that means it's an invalid character //
			else if (current == ' ' || current == '\t' || current == '\r' || current == '\n');

			else
			{
				// Throws an error to alert the user of the invalid character //
				throw InvalidCharInSource(info.index, current);
			}

			// Updates trackers to their default state of a new character //

			info.index++;
			info.wasLastCharAlpha = info.isAlpha;
			info.wasLastCharNumeric = info.isNumeric;
		}

		// Logs the tokens if logging is on //
		if (log != nullptr)
		{
			for (auto& token : tokens)
			{
				if (token.contents.empty() == false)
				{
					SafeLog(log, ToString(token.type), ":\t", token.contents);
				}

				else
				{
					SafeLog(log, ToString(token.type));
				}
			}
		}

		// Shrinks the vector down to minimum size before returning to avoid excess memory being allocated
		tokens.shrink_to_fit();
		return tokens;
	}
}