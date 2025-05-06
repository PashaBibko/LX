#include <LX-Common.h>

#include <Parser.h>
#include <Lexer.h>

#include <../Lexer/inc/LexerErrors.h> // <- TEMP (I hope)

namespace LX
{
	// Different errors thrown by main //
	struct InvalidFilePath : public RuntimeError
	{
		GENERATE_LX_ERROR_REQUIRED_FUNCTION_DECLARATIONS;

		InvalidFilePath(const std::string& _name, const std::filesystem::path& _path)
			: name(_name), path(_path)
		{}

		std::string name;
		std::filesystem::path path;
	};

	void InvalidFilePath::PrintToConsole() const
	{
		// Tells the user the input file could not be found and how to fix the issue //
		LX::PrintStringAsColor("Error: ", LX::Color::LIGHT_RED);
		std::cout << "Invalid " << name << ": ";
		LX::PrintStringAsColor(path.string().c_str(), LX::Color::WHITE);
		std::cout << "\n\nMake sure the file exists and the process has the correct path to the file\n";
	}

	const char* InvalidFilePath::ErrorType() const
	{
		return "Invalid File Path";
	}
}

extern "C" int __declspec(dllexport) GenIR(const char* a_inpPath, const char* a_outPath)
{
	// Initalises the log //
	LX::Log::Init();

	// Creates the file paths outside of the try-catch so they can be used in errors //
	std::filesystem::path inpPath;
	std::filesystem::path outPath;

	// Creates the contents string outside of the try-catch so they can be used in errors //
	std::string contents;
	LX::Token::source = &contents;

	try
	{
		// Turns the file paths into the C++ type for handling them //
		inpPath = a_inpPath;
		outPath = a_outPath;

		// Checks the input file exists and opens it //
		LX::ThrowIf<LX::InvalidFilePath>(std::filesystem::exists(inpPath) == false, "input file path", inpPath);
		std::ifstream inpFile(inpPath, std::ios::binary | std::ios::ate); // Opens in binary at the end for microptimisation //
		LX::ThrowIf<LX::InvalidFilePath>(inpFile.is_open() == false, "input file path", inpPath);

		// Copies the file into the string //
		const std::streamsize len = inpFile.tellg(); // Gets length of file because it was opened at the end
		inpFile.seekg(0, std::ios::beg); // Goes back to the beginning
		contents = std::string(len, '\0'); // Allocates all the space for the string
		inpFile.read(&contents[0], len); // Transfers file contents to string

		// Opens / Creates the output file //
		std::ofstream outFile(outPath);
		LX::ThrowIf<LX::InvalidFilePath>(outFile.is_open() == false, "output file path", outPath);
		outFile.close(); // Opened just to check we can

		// Prints the full paths to the console to let the user know compiling is being done //
		std::cout << std::filesystem::absolute(inpPath) << " -> " << std::filesystem::absolute(outPath) << std::endl;

		// Create tokens out of the input file //
		LX::InvalidCharInSource::s_Source = &contents;
		LX::InvalidCharInSource::s_SourceFile = &inpPath;
		std::vector<LX::Token>tokens = LX::LexicalAnalyze(contents, len);

		// Turns the tokens into an AST //
		LX::UnexpectedToken::s_Source = &contents;
		LX::UnexpectedToken::s_SourceFile = &inpPath;

		LX::FileAST AST = LX::TurnTokensIntoAbstractSyntaxTree(tokens);

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
		// Logs the error. Does not need to close it as it is done after this function returns //
		LX::Log::LogNewSection("std::exception thrown: ", e.what());

		// Prints the std exception to the console //
		// Any errors here are problems with the code //
		std::cout << "An error occured. Please report this on the github page.\n" << std::endl;
		std::cout << e.what() << std::endl;

		// Exit code -1 means an undefined error //
		return -1;
	}

	// Default catches any non-specified errors //
	catch (...)
	{
		// Exit code -1 means an undefined error //
		return -1;
	}
}
