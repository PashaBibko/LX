#include <Common/Macro.h>

#include <Lexer/inc/Lexer.h>

#include <Parser/inc/FuncAST.h>
#include <Parser/inc/Parser.h>

#include <iostream>

int main()
{
	namespace AST = LX::FuncAST;

	// Stops errors
	try
	{
		// Reads the file to a string
		std::string file = LX::ReadFile("D:/repos/LX-Compiler/Tester/Examples/Test.lx");

		// Allocates the memory for the tokens
		LX::TokenOutput tokens;

		// Creates the tokens
		LX::Lexer lexer(file, tokens);

		// The files AST
		LX::FileAST AST;

		// Turns the tokens into an abstract syntax tree (AST)
		LX::Parser parser(tokens, AST);
	}

	// Catches all errors that could be thrown
	catch (std::exception& e)
	{
		// Prints it to the console
		std::cout << e.what() << "\n\n";
		return -1;
	}
}