#include <Lexer.h>
#include <Util.h>

#include <unordered_map>
#include <string_view>
#include <fstream>
#include <vector>

#include <iostream>
#include <iomanip>

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
			TOKEN_CASE(Token::RETURN);

			default:
				return "Unknown: " + std::to_string(type);
		}
	}

	static constexpr bool CanBePartOfNumberLiteral(const char c)
	{
		return (c == '.') || (c == 'f');
	}

	static std::string PrintChar(const char c)
	{
		switch (c)
		{
			case '\n': return R"(\n)";
			case '\t': return R"(\t)";
			case '\r': return R"(\r)";
			default: return std::string(1, c);
		}
	}

	// Struct to store the current information of the lexer //
	struct LexerInfo
	{
		// Current trackers of where in the source it is //

		std::streamsize line = 1; // <- Lines start on 1 (probably because of non-programmer's)
		std::streamsize index = 0;
		std::streamsize column = 0; // <- Columns start on 1 (probably because of non-programmer's)

		// Trackers for when a multi-char token started //

		std::streamsize startOfWord = 0;
		std::streamsize startOfNumberLiteral = 0;
		std::streamsize startOfStringLiteral = 0;

		// Different flags of the lexer //
		// Stored as a bitset to minimse memory allocated (basically no difference, because only one exists at any given time) //

		bool isAlpha : 1 = false;
		bool isNumeric : 1 = false;
		bool inComment : 1 = false;
		bool inStringLiteral : 1 = false;
		bool isNextCharAlpha : 1 = false;
		bool isNextCharNumeric : 1 = false;
		bool wasLastCharAlpha : 1 = false;
		bool wasLastCharNumeric : 1 = false;
		bool lexingNumber : 1 = false;
	};

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
	static void TokenizeWord(const std::string& word, std::vector<Token>& tokens, LexerInfo& info)
	{
		// Checks the map for a check and if so adds it with its enum equivalent //
		if (auto keyword = keywords.find(word); keyword != keywords.end())
		{
			tokens.push_back({ keyword->second, "", info.line, info.column - (std::streamsize)word.size(), (std::streamsize)word.size()});
		}

		// Else adds it as a type of IDENTIFIER //
		else
		{
			tokens.push_back({ Token::IDENTIFIER, word, info.line, info.column - (std::streamsize)word.size(), (std::streamsize)word.size()});
		}
	}

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
					tokens.push_back({ Token::STRING_LITERAL, lit, info.line, info.column - (std::streamsize)lit.length(), (std::streamsize)lit.length() });

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
					tokens.push_back({ Token::NUMBER_LITERAL, num, info.line, info.column - (std::streamsize)num.size(), (std::streamsize)num.size() });
				}

				// Stores it is lexing a number literal //
				else { info.lexingNumber = true; }
			}

			// End of a number //
			else if ((info.isNumeric == true || CanBePartOfNumberLiteral(current)) && info.isNextCharNumeric == false && info.lexingNumber == true)
			{
				// Pushes the number to the token vector. Number literals are stored as string in the tokens //
				std::string num(contents.data() + info.startOfNumberLiteral, (unsigned __int64)(info.index + 1) - info.startOfNumberLiteral);
				tokens.push_back({ Token::NUMBER_LITERAL, num, info.line, info.column - (std::streamsize)num.size(), (std::streamsize)num.size() });
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

			// Operators (+, -, /, *) //
			else if (auto op = operators.find(current); op != operators.end())
			{
				tokens.push_back({ op->second, "", info.line, info.column, 1});
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

			// Throws an error with all the relevant information //s
			else
			{
				// Finds the start of the line //
				size_t start = contents.rfind('\n', info.index);
				if (start == std::string::npos) { start = 0; } // std::npos means none was found so defaults to 1
				else { start = start + 1; } // Skips the new line character

				// Finds the end of the line //
				size_t end = contents.find('\n', info.index);
				if (end == std::string::npos) { end = contents.size(); } // If it reaches the end with no /n it defaults to the length of the string

				// The line where the invalid character is //
				std::string line = contents.substr(start, end - start);

				// Throws an error to alert the user of the invalid character //
				throw InvalidCharInSource(info.column, info.line, line, contents[info.index]);
			}

			// Log dumps A LOT of info //

			SafeLog
			(
				log,
				"Is Alpha: ", info.isAlpha,
				" Is Numeric: ", info.isNumeric,
				" In Comment: ", info.inComment,
				" In String: ", info.inStringLiteral,
				" Next Char Alpha: ", info.isNextCharAlpha,
				" Next Char Numeric: ", info.wasLastCharNumeric,
				" Last Char Numeric: ", info.wasLastCharAlpha,
				" Lexing number: ", info.lexingNumber,
				" Current: {", PrintChar(current), "}"
			);

			// Updates trackers to their default state of a new character //

			info.index++;
			info.column++;

			info.wasLastCharAlpha = info.isAlpha;
			info.wasLastCharNumeric = info.isNumeric;
		}

		// Logs the tokens if logging is on //
		if (log != nullptr)
		{
			SafeLog(log, LOG_BREAK, "Tokens", LOG_BREAK);

			for (auto& token : tokens)
			{
				if (token.contents.empty() == false)
				{
					SafeLog(log, "{ Line: ", std::left, std::setw(3), token.line, ", Column: ", std::setw(3), token.index, ", Length: ", std::setw(2), token.length, "} ", std::setw(30), ToString(token.type) + ":", "{", token.contents, "}");
				}

				else
				{
					SafeLog(log, "{ Line: ", std::left, std::setw(3), token.line, ", Column: ", std::setw(3), token.index, ", Length: ", std::setw(2), token.length, "} ", ToString(token.type));
				}
			}

			SafeLog(log, "\n END OF TOKENS");
		}

		// Shrinks the vector down to minimum size before returning to avoid excess memory being allocated
		tokens.shrink_to_fit();
		return tokens;
	}
}