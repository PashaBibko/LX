#include <Common/Macro.h>

#include <Lexer/inc/Lexer.h>

#include <Parser/inc/FuncAST.h>

#include <iostream>

int main()
{
	try
	{
		// Reads the file to a string
		std::string file = LX::ReadFile("D:/repos/LX-Compiler/Tester/Examples/Test.lx");

		// Allocates the memory for the tokens
		std::vector<LX::EmptyTokenSection> tokens;

		// Creates the tokens
		LX::Lexer l(file, tokens);
	}

	// Catches all errors that could be thrown
	catch (std::exception& e)
	{
		// Prints it to the console
		std::cout << e.what() << "\n\n";
		return -1;
	}
}