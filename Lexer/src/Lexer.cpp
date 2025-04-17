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

			default:
				return "Unknown: " + std::to_string(type);
		}
	}

	static const std::unordered_map<std::string, Token::TokenType> keywords =
	{
		{ "for"			, Token::FOR		},
		{ "while"		, Token::WHILE		},
		{ "if"			, Token::IF			},
		{ "else"		, Token::ELSE		},
		{ "elif"		, Token::ELIF		},
		{ "func"		, Token::FUNCTION	},
	};

	static const std::unordered_map<char, Token::TokenType> operators =
	{
		{ '+', Token::ADD },
		{ '-', Token::SUB },
		{ '*', Token::MUL },
		{ '/', Token::DIV }
	};

	static void TokenizeWord(const std::string& word, std::vector<Token>& tokens)
	{
		if (auto keyword = keywords.find(word); keyword != keywords.end())
		{
			tokens.push_back({ keyword->second, word });
		}

		else
		{
			tokens.push_back({ Token::IDENTIFIER, word });
		}
	}

	const std::vector<Token> LX::LexicalAnalyze(std::ifstream& src, std::ofstream* log)
	{
		// Logs the start of the lexical analysis
		SafeLog(log, LOG_BREAK, "Started lexing file", LOG_BREAK);

		// Allocates a large ammount of memory to hold the output
		// Will shrink the size later on to stop excess memory
		std::vector<Token> tokens = {};
		tokens.reserve(0xFFFF);

		// Turns the contents of the file into a string //

		// Gets length of the file because it is opened at the end
		const std::streamsize len = src.tellg();
		src.seekg(0, std::ios::beg); // Goes back to the beginning

		std::string contents(len, '\0'); // Preallocates all space needed
		src.read(&contents[0], len); // Transfers file to string

		// Trackers for when the program is iterating over the file //

		std::streamsize index = 0;

		std::streamsize startOfWord = 0;
		std::streamsize startOfStringLiteral = 0;

		bool isAlpha = false;
		bool inComment = false;
		bool inStringLiteral = false;
		bool wasLastCharAlpha = false;
		
		// Iterates over the file and turns it into tokens //
		while (index < len)
		{
			// Stores the current character for easy access
			const char current = contents[index];

			// Works out if the current character is alphabetic
			isAlpha = (current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z');

			// Updates string literal tracker and skips over rest if in a string literal
			if (current == '"')
			{
				// Start of string literal
				if (inStringLiteral == false)
				{
					// Updates the neccesarry trackers
					startOfStringLiteral = index + 1;
					inStringLiteral = true;
				}

				// End of string literal
				else
				{
					// Adds the string literal token to the token vector
					std::string lit(contents.data() + startOfStringLiteral, index - startOfStringLiteral);
					tokens.push_back({ Token::STRING_LITERAL, lit });

					// Updates trackers
					inStringLiteral = false;
				}
			}

			// Skips over rest if within a string literal
			else if (inStringLiteral);

			// Updates comment state
			else if (current == '#')
			{
				inComment = !inComment;
			}

			// Skips over if within a comment
			else if (inComment);

			// Start of a word
			else if (isAlpha == true && wasLastCharAlpha == false)
			{
				startOfWord = index;
			}

			// During a word
			else if (isAlpha == true);

			// Operators (+, -, /, *)
			else if (auto op = operators.find(current); op != operators.end())
			{
				tokens.push_back({ op->second, "" });
			}

			// If it is here and not whitespace that means it's an invalid character
			else if (current == ' ' || current == '\t' || current == '\r' || current == '\n');

			else
			{
				// Throws an error to alert the user
				throw InvalidCharInSource(index, current);
			}

			// End of a word
			if (isAlpha == false && wasLastCharAlpha == true)
			{
				TokenizeWord({ contents.data() + startOfWord, (unsigned __int64)(index - startOfWord) }, tokens);
			}

			// Updates trackers //

			index++;
			wasLastCharAlpha = isAlpha;
		}

		// Words are only added the iteration after they end so it has to be done like this //
		if (wasLastCharAlpha && isAlpha)
		{
			std::string word(contents.data() + startOfWord, index - startOfWord);
			TokenizeWord(word, tokens);
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