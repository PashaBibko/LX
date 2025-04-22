#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include <Console.h>
#include <Parser.h>
#include <Lexer.h>
#include <Util.h>

namespace LX
{
	// Different errors thrown by main //

	struct IncorrectCommandLineArgs {};
	struct InvalidInputFilePath {};
	struct InvalidOutputFilePath {};
	struct InvalidLogFilePath {};

	// Util function for getting a line of the source at a given index (used for errors) //
	static std::string GetLineAtIndexOf(const std::string src, const std::streamsize index)
	{
		// Finds the start of the line //
		size_t start = src.rfind('\n', index);
		if (start == std::string::npos) { start = 0; } // None means first line
		else { start = start + 1; } // Skips new line char

		// Finds the end of the line //
		size_t end = src.find('\n', index);
		if (end == std::string::npos) { end = src.size(); } // None means last line

		// Returns the string between start and end //
		return src.substr(start, end - start);
	}
}

int main(int argc, char** argv)
{
	// Creates the file paths outside of the try-catch so they can be used in errors //
	std::filesystem::path inpPath;
	std::filesystem::path outPath;
	std::filesystem::path logPath;

	// Creates the contents string outside of the try-catch so they can be used in errors //
	std::string contents;
	LX::Token::source = &contents;

	// Creates the log-file out of the try-catch so it can be closed propely if an error is thrown //
	std::unique_ptr<std::ofstream> log = nullptr;

	try
	{
		// Checks there is the correct ammount of arguments //
		LX::ThrowIf<LX::IncorrectCommandLineArgs>((argc == 3 || argc == 4) == false);

		// Turns the file paths into the C++ type for handling them //
		inpPath = argv[1];
		outPath = argv[2];

		// Checks the input file exists and opens it //
		LX::ThrowIf<LX::InvalidInputFilePath>(std::filesystem::exists(inpPath) == false);
		std::ifstream inpFile(inpPath, std::ios::binary | std::ios::ate); // Opens in binary at the end for microptimisation //
		LX::ThrowIf<LX::InvalidInputFilePath>(inpFile.is_open() == false);

		// Copies the file into the string //
		const std::streamsize len = inpFile.tellg(); // Gets length of file because it was opened at the end
		inpFile.seekg(0, std::ios::beg); // Goes back to the beginning
		contents = std::string(len, '\0'); // Allocates all the space for the string
		inpFile.read(&contents[0], len); // Transfers file contents to string

		// Opens / Creates the output file //
		std::ofstream outFile(outPath);
		LX::ThrowIf<LX::InvalidOutputFilePath>(outFile.is_open() == false);
		outFile.close(); // Opened just to check we can

		// Opens the log file (if there is one specified //
		if (argc == 4)
		{
			logPath = argv[3];
			log = std::make_unique<std::ofstream>(logPath);
			LX::ThrowIf<LX::InvalidLogFilePath>(log->is_open() == false);
		}

		// Prints the full paths to the console to let the user know compiling is being done //
		std::cout << std::filesystem::absolute(inpPath) << " -> " << std::filesystem::absolute(outPath) << std::endl;

		// Create tokens out of the input file //
		std::vector<LX::Token>tokens = LX::LexicalAnalyze(contents, len, log.get());
		LX::SafeFlush(log.get());

		// Turns the tokens into an AST //
		LX::FileAST AST = LX::TurnTokensIntoAbstractSyntaxTree(tokens, log.get());
		LX::SafeFlush(log.get());

		// Turns the AST into LLVM IR //
		LX::GenerateIR(AST, inpPath.filename().string(), outPath);
		LX::SafeFlush(log.get());

		// Returns success
		return 0;
	}

	catch (LX::IncorrectCommandLineArgs)
	{
		// Prints out how to correctly use the program //
		LX::PrintStringAsColor("Error: ", LX::Color::LIGHT_RED);
		std::cout << "Incorrect use of " << std::filesystem::path(argv[0]).filename().string() << ":\n\n";
		std::cout << "Usage: ";
		LX::PrintStringAsColor("[source file] [output file] [optional args]", LX::Color::WHITE);
		std::cout << "\n\nOptional arguments:\n";
		std::cout << "\tLog file for additional information\n\n";

		// Warns the user that they are better of using a build system //
		LX::PrintStringAsColor("Warning:\n", LX::Color::LIGHT_YELLOW);
		std::cout << "\tIf you are seeing this message it is probably because you are not using a build-system\n";
		std::cout << "\tWorking with a build system is recommended for use of " << std::filesystem::path(argv[0]).filename().string() << "\n";
		std::cout << "\tOne can be found here: NULL\n\n"; // <- TODO: Make a build system

		// Returns Exit id of 1 so other process can be alerted of the error //
		return 1;
	}

	catch (LX::InvalidInputFilePath)
	{
		// Tells the user the input file could not be found and how to fix the issue //
		LX::PrintStringAsColor("Error: ", LX::Color::LIGHT_RED);
		std::cout << "Invalid file path: ";
		LX::PrintStringAsColor(inpPath.string(), LX::Color::WHITE);
		std::cout << "\n\nMake sure the file exists and the process has the correct path to the file\n";

		// Returns Exit id of 2 so other process can be alerted of the error //
		return 2;
	}

	catch (LX::InvalidOutputFilePath)
	{
		// Tells the user that the output file could not be found/created //
		LX::PrintStringAsColor("Error: ", LX::Color::LIGHT_RED);
		std::cout << "Invalid file path: ";
		LX::PrintStringAsColor(outPath.string(), LX::Color::WHITE);
		std::cout << "\n\nThe file could not be created or written to.\n";
		std::cout << "Check it is a valid file path and it has the permissions to modify the file\n";

		// Returns Exit id of 3 so other process can be alerted of the error //
		return 3;
	}

	catch (LX::InvalidLogFilePath)
	{
		// Tells the user that the log file cound not be found/created //
		LX::PrintStringAsColor("Error: ", LX::Color::LIGHT_RED);
		std::cout << "Invalid file path: ";
		LX::PrintStringAsColor(logPath.string(), LX::Color::WHITE);
		std::cout << "\n\nThe file could not be created or written to.\n";
		std::cout << "Check it is a valid file path and it has the permissions to modify the file\n";

		// Returns Exit id of 4 so other process can be alerted of the error //
		return 4;
	}

	catch (LX::InvalidCharInSource& e)
	{
		// Adds the error to the log and closes it to save all the output //
		LX::SafeLog(log.get(), LX::LOG_BREAK, "Error thrown from Lexer:\n\tInvalid character: ", e.invalid, " on line: ", e.line, LX::LOG_BREAK);
		if (log != nullptr) { log->close(); }

		// Calculates the length of the line number in the console so it is formatted correctly //
		std::ostringstream oss;
		oss << std::setw(3) << e.line;
		size_t lineNumberWidthInConsole = std::max(oss.str().size(), (size_t)3);

		// Gets the line of the error //
		std::string line = LX::GetLineAtIndexOf(contents, e.index);

		// Prints the error with the relevant information to the console //
		std::cout << "\n";
		LX::PrintStringAsColor("Error: ", LX::Color::LIGHT_RED);
		std::cout << "Invalid character found in ";
		LX::PrintStringAsColor(inpPath.filename().string(), LX::Color::WHITE);
		std::cout << ":\n";
		std::cout << "Line: " << std::setw(lineNumberWidthInConsole) << e.line << " | " << line << "\n";
		std::cout << "      " << std::setw(lineNumberWidthInConsole) << "" << " | " << std::setw(e.col);
		LX::PrintStringAsColor("^", LX::Color::LIGHT_RED);
		std::cout << "\n";

		// Returns Exit id of 5 so other process can be alerted of the error //
		return 5;
	}

	catch (LX::UnexpectedToken& e)
	{
		// Calculates the length of the line number in the console so it is formatted correctly //
		std::ostringstream oss;
		oss << std::setw(3) << e.got.line;
		size_t lineNumberWidthInConsole = std::max(oss.str().size(), (size_t)3);

		// Gets the line of the error //
		std::string line = LX::GetLineAtIndexOf(contents, e.got.index);

		// Prints the error to the console with the relevant info //
		std::cout << "\n";
		LX::PrintStringAsColor("Error: ", LX::Color::LIGHT_RED);
		std::cout << "Incorrect syntax in ";
		LX::PrintStringAsColor(inpPath.filename().string(), LX::Color::WHITE);
		std::cout << ", found ";
		LX::PrintStringAsColor(LX::ToString(e.got.type), LX::Color::WHITE);
		std::cout << " expected: ";

		// Allows the error to have a custom type that is printed to the console //
		if (e.expected == LX::Token::UNDEFINED) { LX::PrintStringAsColor(e.override, LX::Color::WHITE); }
		else { LX::PrintStringAsColor(LX::ToString(e.expected), LX::Color::WHITE); }
		std::cout << "\n";

		// Prints the code with the error to the console //
		std::string errorSquiggle(e.got.length, '~');
		std::cout << "Line: " << std::setw(lineNumberWidthInConsole) << e.got.line << " | " << line << "\n";
		std::cout << "      " << std::setw(lineNumberWidthInConsole) << "" << " | " << std::setw(e.got.column) << "";
		LX::PrintStringAsColor(errorSquiggle, LX::Color::LIGHT_RED);
		std::cout << "\n";

		// Returns Exit id of 6 so other process can be alerted of the error //
		return 6;
	}

	// Catches any std errors, there should be none //
	catch (std::exception& e)
	{
		// Prints the std exception to the console //
		// Any errors here are problems with the code //
		std::cout << e.what() << std::endl;
	}

	// Default catches any non-specified errors //
	catch (...) {}

	// Closes the log if it is open to get as much info as possible //
	if (log != nullptr) { log->close(); }
	std::cout << "An unknown error occured. Please report this on the github page.\n";
	return -1; // -1 exit code means an unknown error
}
