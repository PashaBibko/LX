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
}

int main(int argc, char** argv)
{
	// Creates the file paths outside of the try-catch so they can be used in errors //
	std::filesystem::path inpPath;
	std::filesystem::path outPath;

	// Creates the log-file out of the try-catch so it can be closed propely if an error is thrown //
	std::unique_ptr<std::ofstream> log = nullptr;

	try
	{
		// Checks there is the correct ammount of arguments //
		LX::ThrowIf<LX::IncorrectCommandLineArgs>((argc == 3 || argc == 4) == false);

		// Turns the file paths into the C++ type for handling them //
		inpPath = argv[1];
		outPath = argv[2];

		// Prints the full paths to the console to let the user know compiling is being done //
		std::cout << std::filesystem::absolute(inpPath) << " -> " << std::filesystem::absolute(outPath) << std::endl;

		// Checks the input file exists and opens it //
		LX::ThrowIf<LX::InvalidInputFilePath>(std::filesystem::exists(inpPath) == false);
		std::ifstream inpFile(inpPath, std::ios::binary | std::ios::ate); // Opens in binary at the end for microptimisation //
		LX::ThrowIf<LX::InvalidInputFilePath>(inpFile.is_open() == false);

		// Opens / Creates the output file //
		std::ofstream outFile(outPath);
		LX::ThrowIf<LX::InvalidOutputFilePath>(outFile.is_open() == false);

		// Opens the log file (if there is one specified //
		if (argc == 4)
		{
			log = std::make_unique<std::ofstream>(argv[3]);
			LX::ThrowIf<LX::InvalidLogFilePath>(log->is_open() == false);
		}

		// Create tokens out of the input file //
		std::vector<LX::Token>tokens = LX::LexicalAnalyze(inpFile, log.get());

		// Turns the tokens into an AST //
		LX::FileAST AST = LX::TurnTokensIntoAbstractSyntaxTree(tokens, log.get());

		// Turns the AST into LLVM IR //
		LX::GenerateIR(AST, inpPath.filename().string());

		// Returns success
		return 0;
	}

	catch (LX::IncorrectCommandLineArgs)
	{
		// Displays to the console of how to use the program
		std::cout << "\nUsage: [source file] [output file] (optional)[log file]\n";

		return 1;
	}

	catch (LX::InvalidInputFilePath)
	{
		// Tells user the input file could not be found
		std::cout << "\nFile path: {" << argv[1] << "} could not be opened\n";

		return 2;
	}

	catch (LX::InvalidOutputFilePath)
	{
		// Tells the user the output file could not be opened
		std::cout << "\nCould not open/create {" << argv[2] << "}\n";

		return 3;
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

		// Prints the error with the relevant information to the console //
		std::cout << "\n";
		LX::PrintStringAsColor("Error: ", LX::Color::LIGHT_RED);
		std::cout << "invalid character found in ";
		LX::PrintStringAsColor(inpPath.filename().string(), LX::Color::WHITE);
		std::cout << ":\n";
		std::cout << "Line: " << std::setw(lineNumberWidthInConsole) << e.line << " | " << e.lineContents << "\n";
		std::cout << "      " << std::setw(lineNumberWidthInConsole) << ""     << " | " << std::setw(e.index);
		LX::PrintStringAsColor("^", LX::Color::LIGHT_RED);
		std::cout << "\n";

		// Returns Exit-id of 4 so other process can be alerted of the error //
		return 4;
	}

	catch (std::exception& e)
	{
		// Prints the std exception to the console
		std::cout << "\nAn error occured:\n" << e.what() << std::endl;

		return 5;
	}

	catch (...)
	{
		// Tells the user if an error has happened
		std::cout << "\nAn Error occured\n";

		return 6;
	}
	
	// -1 means an error slipped through (IDK how, it's here just in case)
	return -1;
}
