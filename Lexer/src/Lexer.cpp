#include <LX-Common.h>

#include <Lexer.h>

#include <LexerErrors.h>
#include <LexerInfo.h>
#include <Constants.h>

namespace LX
{
	// Helper function for dealing with floating-point number literals //
	static constexpr bool CanBePartOfNumberLiteral(const char c) { return (c == '.') || (c == 'f'); }

	// Checks if the given word is a keyword before adding it to the tokens //
	static void TokenizeWord(const std::string& word, std::vector<Token>& tokens, LexerInfo& info)
	{
		// Checks the map for a check and if so adds it with its enum equivalent //
		if (auto keyword = keywords.find(word); keyword != keywords.end())
		{
			tokens.push_back({ keyword->second, info, (std::streamsize)word.size(), info.source });
		}

		// Else adds it as a type of IDENTIFIER //
		else
		{
			tokens.push_back({ Token::IDENTIFIER, info, (std::streamsize)word.size(), info.source });
		}
	}

	static inline void UpdateLexerInfo(LexerInfo& info)
	{
		// Transfers the previous state if not at the beginning //
		if (info.index != 0) [[likely]]
		{
			info.wasLastCharAlpha = info.isAlpha;
			info.wasLastCharNumeric = info.isNumeric;

			info.isAlpha = info.isNextCharAlpha;
			info.isNumeric = info.isNextCharNumeric;
		}

		else
		{
			// Stores the current character for easy access
			const char current = info.source[info.index];

			// Works out if the current character is alphabetic or numeric //
			info.isAlpha = (current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z');
			info.isNumeric = (current >= '0' && current <= '9');
		}

		// Only does next character checks when not at the end //
		if (info.index + 1 < info.len) [[likely]]
		{
			// Gets the next character //
			const char next = info.source[info.index + 1];

			// Sets flags depending on the value of the next character //
			info.isNextCharAlpha = (next >= 'a' && next <= 'z') || (next >= 'A' && next <= 'Z');
			info.isNextCharNumeric = (next >= '0' && next <= '9') || CanBePartOfNumberLiteral(next);
		}

		// Else defaults the flags to false //
		else
		{
			info.isNextCharAlpha = false;
			info.isNextCharNumeric = false;
		}
	}

	const std::vector<Token> LX::LexicalAnalyze(const std::filesystem::path& path)
	{
		// Logs that the file is being read //
		Log::LogNewSection("Reading file: ", path.string());

		std::string fileContents = ReadFileToString(path);
		const std::streamsize len = fileContents.length();

		// Logs the start of the lexical analysis
		Log::LogNewSection("Lexing file");

		// Allocates a large ammount of memory to hold the output //
		// Will shrink the size later on to stop excess memory being allocated //
		std::vector<Token> tokens = {};
		tokens.reserve(0xFFFF);

		// Trackers for when the program is iterating over the file //
		LexerInfo info(fileContents);

		// Iterates over the file and turns it into tokens //
		while (info.index < len)
		{
			// Stores the current character for easy access
			const char current = info.source[info.index];

			// Updates the LexerInfo //
			UpdateLexerInfo(info);

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
					std::string lit(info.source.data() + info.startOfStringLiteral, info.index - info.startOfStringLiteral);
					tokens.push_back({ Token::STRING_LITERAL, info, (std::streamsize)lit.length() + 2, info.source }); // Adding two makes the "" be stored as well

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
					std::string num(info.source.data() + info.startOfNumberLiteral, (unsigned __int64)(info.index + 1) - info.startOfNumberLiteral);
					tokens.push_back({ Token::NUMBER_LITERAL, info, (std::streamsize)num.size(), info.source });
				}

				// Stores it is lexing a number literal //
				else { info.lexingNumber = true; }
			}

			// End of a number //
			else if ((info.isNumeric == true || CanBePartOfNumberLiteral(current)) && info.isNextCharNumeric == false && info.lexingNumber == true)
			{
				// Pushes the number to the token vector. Number literals are stored as string in the tokens //
				std::string num(info.source.data() + info.startOfNumberLiteral, (unsigned __int64)(info.index + 1) - info.startOfNumberLiteral);
				tokens.push_back({ Token::NUMBER_LITERAL, info, (std::streamsize)num.size(), info.source });
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
					TokenizeWord({ info.source.data() + info.startOfWord, 1 }, tokens, info);
				}
			}

			// End of a word //
			else if (info.isAlpha == true && info.isNextCharAlpha == false)
			{
				// Calls the function designed to handle the tokenisation of words //
				TokenizeWord({ info.source.data() + info.startOfWord, (unsigned __int64)((info.index + 1) - info.startOfWord) }, tokens, info);
			}

			// During a word //
			else if (info.isAlpha == true);

			// Symbols //
			else if (auto sym = symbols.find(current); sym != symbols.end())
			{
				tokens.push_back({ sym->second, info, 1, info.source });
			}

			// Operators (+, -, /, *) //
			else if (auto op = operators.find(current); op != operators.end())
			{
				tokens.push_back({ op->second, info, 1, info.source });
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
				ThrowIf<InvalidCharInSource>(true, info, path.string());
			}

			// Log dumps A LOT of info //

			Log::out
			(
				"Index: ", std::left, std::setw(3), info.index,
				" Is Alpha: ", info.isAlpha,
				" Is Numeric: ", info.isNumeric,
				" In Comment: ", info.inComment,
				" In String: ", info.inStringLiteral,
				" Next Char Alpha: ", info.isNextCharAlpha,
				" Next Char Numeric: ", info.wasLastCharNumeric,
				" Last Char Numeric: ", info.wasLastCharAlpha,
				" Lexing number: ", info.lexingNumber,
				" Current: {", CharAsStrLit(current), "}"
			);

			// Updates the indecies to the next character //

			info.index++;
			info.column++;
		}

		Log::out("\n"); // Puts a space to clean up the log

		for (auto& token : tokens)
		{
			Log::out<Log::Priority::HIGH>
			(
				std::left,
				"{ Line: ", std::setw(3), token.line,
				", Index: ", std::setw(3), token.index,
				", Length: ", std::setw(2), token.length, " } ",
				std::setw(30), ToString(token.type) + ":", "{", token.GetContents(), "}"
			);
		}

		Log::out("End of tokens");

		// Shrinks the vector down to minimum size before returning to avoid excess memory being allocated
		tokens.shrink_to_fit();
		return tokens;
	}
}