#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include <llvm/Support/Error.h>

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

extern "C" int __declspec(dllexport) GenIR(const char* a_inpPath, const char* a_outPath, const char* a_logPath)
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
		// Turns the file paths into the C++ type for handling them //
		inpPath = a_inpPath;
		outPath = a_outPath;

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
		if (a_logPath != nullptr)
		{
			logPath = a_logPath;
			log = std::make_unique<std::ofstream>(logPath);
			LX::ThrowIf<LX::InvalidLogFilePath>(log->is_open() == false);
		}

		// Prints the full paths to the console to let the user know compiling is being done //
		std::cout << std::filesystem::absolute(inpPath) << " -> " << std::filesystem::absolute(outPath) << std::endl;

		// Create tokens out of the input file //
		std::vector<LX::Token>tokens = LX::LexicalAnalyze(contents, len, log.get());
		LX::SafeFlush(log.get());
		std::cout << "\t|- Created tokens" << std::endl;

		// Turns the tokens into an AST //
		LX::FileAST AST = LX::TurnTokensIntoAbstractSyntaxTree(tokens, log.get());
		LX::SafeFlush(log.get());
		std::cout << "\t|- Created AST" << std::endl;

		// Turns the AST into LLVM IR //
		LX::GenerateIR(AST, inpPath.filename().string(), outPath);
		LX::SafeFlush(log.get());
		std::cout << "\t|- Generated LLVM IR" << std::endl;

		// Returns success
		return 0;
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
		std::cout << " {";
		LX::PrintStringAsColor(std::string(1, e.invalid), LX::Color::LIGHT_RED);
		std::cout << "}:\n";
		std::cout << "Line: " << std::setw(lineNumberWidthInConsole) << e.line << " | " << line << "\n";
		std::cout << "      " << std::setw(lineNumberWidthInConsole) << "" << " | " << std::setw(e.col - 1) << "";
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

	catch (LX::Scope::VariableAlreadyExists)
	{
		std::cout << "Tried to create a variable that already exists\n";

		return 7;
	}

	catch (LX::Scope::VariableDoesntExist)
	{
		std::cout << "Tried to access a variable that doesn't exist\n";

		return 8;
	}

	// Catches any std errors, there should be none //
	catch (std::exception& e)
	{
		// Closes the log if it is open to get as much info as possible //
		if (log != nullptr) { log->close(); }

		// Prints the std exception to the console //
		// Any errors here are problems with the code //
		std::cout << "An error occured. Please report this on the github page.\n" << std::endl;
		std::cout << e.what() << std::endl;

		// Exit code -1 means an undefined error //
		return -1;
	}

	// Catches any LLVM errors, there should be none //
	catch (llvm::Error& e)
	{
		// Closes the log if it is open to get as much info as possible //
		if (log != nullptr) { log->close(); }

		// Prints the LLVM error to the console //
		std::cout << "A LLVM error occured. Please report this on the github page.\n" << std::endl;

		// Exit code -1 means an undefined error //
		return -1;
	}

	// Catches errors that i was too lazy to code //
	catch (int)
	{
		// Closes the log if it is open to get as much info as possible //
		if (log != nullptr) { log->close(); }

		std::cout << "An placeholder error occured. Maybe use a language that wasn't coded by a lazy person.\n" << std::endl;

		// Exit code -1 means an undefined error //
		return -1;
	}

	// Default catches any non-specified errors //
	catch (...)
	{
		// Closes the log if it is open to get as much info as possible //
		if (log != nullptr) { log->close(); }

		// Exit code -1 means an undefined error //
		return -1;
	}
}
