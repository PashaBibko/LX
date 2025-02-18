#include <Lexer/inc/File.h>

#include <fstream>
#include <string>

static std::string GetRawContents(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if (!file)
	{
		throw std::runtime_error("Could not open file: " + filename);
	}

	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::string buffer(size, '\0');
	file.read(buffer.data(), size);

	return buffer;
}

static void PruneFileContents(std::string& fileContents)
{
	// The state of the pruning
	bool inQuotes = false;
	bool wasSpaceLastIteration = false;

	size_t write = 0;

	// Iterates over the string
	for (size_t read = 0; read < fileContents.length(); ++read)
	{
		// Toggles quote state if needed
		if (fileContents[read] == '"')
		{
			inQuotes = !inQuotes;
		}

		// Checks if currently in whitespace
		if (std::isspace(fileContents[read]))
		{
			if (inQuotes)
			{
				fileContents[write++] = fileContents[read];
			}

			else if (wasSpaceLastIteration == false)
			{
				fileContents[write++] = ' ';
			}

			wasSpaceLastIteration = true;
		}

		else
		{
			fileContents[write++] = fileContents[read];
			wasSpaceLastIteration = false;
		}
	}

	// Resizes to clean extra memory
	fileContents.resize(write);
}

std::string ReadFile(const std::string& filename)
{
	std::string contents = GetRawContents(filename);

	PruneFileContents(contents);

	return contents;
}