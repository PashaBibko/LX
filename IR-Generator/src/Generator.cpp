#include <LX-Common.h>

#include <Parser.h>
#include <Lexer.h>

extern "C" int __declspec(dllexport) GenIR(const char* a_inpPath, const char* a_outPath)
{
	try
	{
		// Initalises the log //
		LX::Log::Init();

		// Turns the file paths into the C++ type for handling them //
		std::filesystem::path inpPath = a_inpPath;
		std::filesystem::path outPath = a_outPath;

		// Opens / Creates the output file //
		std::ofstream outFile(outPath);
		LX::ThrowIf<LX::InvalidFilePath>(outFile.is_open() == false, "output file path", outPath);
		outFile.close(); // Opened just to check we can

		// Prints the full paths to the console to let the user know compiling is being done //
		std::cout << std::filesystem::absolute(inpPath) << " -> " << std::filesystem::absolute(outPath) << std::endl;

		// Create tokens out of the input file //
		std::vector<LX::Token>tokens = LX::LexicalAnalyze(inpPath);

		// Turns the tokens into an AST //
		LX::FileAST AST = LX::TurnTokensIntoAbstractSyntaxTree(tokens, inpPath);

		// Turns the AST into LLVM IR //
		LX::GenerateIR(AST, inpPath.filename().string(), outPath);

		// Returns success
		return 0;
	}

	catch(LX::RuntimeError& e)
	{
		// Logs the error. Does not need to close it as it is done after this function returns //
		LX::Log::LogNewSection("Error thrown of type: ", e.ErrorType());

		// Logs the errors type to the console if built as Debug //
		#ifdef _DEBUG
		std::cout << "LX::RuntimeError thrown of type: ";
		LX::PrintStringAsColor(e.ErrorType(), LX::Color::WHITE);
		std::cout << "\n";
		#endif // _DEBUG 

		// Prints the error to the console and returns //
		e.PrintToConsole();
		return -1;
	}

	// Catches any std errors, there should be none //
	catch (std::exception& e)
	{
		// If using a debugger, throws a breakpoint so it can be caught //
		if (IsDebuggerPresent())
		{
			__debugbreak();
		}

		// Logs the error. Does not need to close it as it is done after this function returns //
		LX::Log::LogNewSection("std::exception thrown: ", e.what());

		// Prints the std exception to the console //
		// Any errors here are problems with the code //
		std::cout << "An error occured. Please report this on the github page.\n" << std::endl;
		std::cout << e.what() << std::endl;

		// Exit code -1 means an undefined error // But this isn't undefined and neither is LX::RuntimeError?
		return -1;
	}

	// Default catches any non-specified errors //
	catch (...)
	{
		// If using a debugger, throws a breakpoint so it can be caught //
		if (IsDebuggerPresent())
		{
			__debugbreak();
		}

		// Exit code -1 means an undefined error //
		return -1;
	}
}
