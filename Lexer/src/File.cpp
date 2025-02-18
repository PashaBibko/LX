#include <Lexer/inc/File.h>

#include <filesystem>
#include <fstream>
#include <string>

static bool IsWhitespace(const char c)
{
	return c == ' '  ||
		   c == '\t' ||
		   c == '\0';
}

std::string ReadFile(const std::string& filename)
{
	// Opens the file in binary mode for faster access
	std::ifstream file(filename, std::ios::binary);

	// Verifies file state
	if (file.is_open() == false)
	{
		// Checks file exists
		if (std::filesystem::exists(filename))
		{
			throw std::runtime_error("File does not exist: " + filename);
		}

		// Else throws a seperate error
		else
		{
			throw std::runtime_error("Could not open file: " + filename);
		}
	}

	// String to hold the output
	std::string output;
	output.reserve(1024); // Inital size to avoid excess allocations

	// State of the parsing
	bool inQuotes = false;
	bool wasSpaceLastIteration = false;

	// Current char of the file
	char current = {};

	// Loops over the file
	while (file.get(current))
	{
		// Toggles quote state if needed
		if (current == '"')
		{
			inQuotes = !inQuotes;
		}

		// Makes multiple whitespace characters act as one
		if (IsWhitespace(current))
		{
			if (inQuotes)
			{
				output += current;
			}

			else if (!wasSpaceLastIteration)
			{
				output += ' ';
			}

			wasSpaceLastIteration = true;
		}

		// Makes new line characters act as whitespace
		else if (current == '\n')
		{
			output += '\n';
			wasSpaceLastIteration = true;
		}

		// Else just appends the new character to the string
		else
		{
			output += current;
			wasSpaceLastIteration = false;
		}
	}

	// Returns the processed output
	return output;
}
