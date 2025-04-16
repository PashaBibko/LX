#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>

#include <Util.h>

namespace LX
{
	// Different errors thrown by main //

	struct IncorrectCommandLineArgs {};
	struct InvalidInputFilePath {};
	struct InvalidOutputFilePath {};
}

int main(int argc, char** argv)
{
	try
	{
		// Checks there is the correct ammount of arguments
		LX::ThrowIf<LX::IncorrectCommandLineArgs>(argc != 3);

		// Turns the file paths into the C++ type for handling them
		std::filesystem::path inpPath = argv[1];
		std::filesystem::path outPath = argv[2];

		// Prints the full paths to the console to let the user know compiling is being done
		std::cout << std::filesystem::absolute(inpPath) << " -> " << std::filesystem::absolute(outPath) << std::endl;

		// Checks the input file exists
		LX::ThrowIf<LX::InvalidInputFilePath>(std::filesystem::exists(inpPath) == false);

		// Opens / Creates the output file and checks if it is open
		std::ofstream outFile(outPath);
		LX::ThrowIf<LX::InvalidOutputFilePath>(outFile.is_open() == false);
	}

	catch (LX::IncorrectCommandLineArgs)
	{
		// Displays to the console of how to use the program
		std::cout << "\nUsage: [source file] [output file]\n";

		return 1;
	}

	catch (LX::InvalidInputFilePath)
	{
		// Tells user the input file could not be found
		std::cout << "\nFile path: {" << argv[1] << "} could not be found\n";

		return 2;
	}

	catch (LX::InvalidOutputFilePath)
	{
		// Tells the user the output file could not be opened
		std::cout << "\nCould not open/create {" << argv[2] << "}";
	}

	return 0;
}
