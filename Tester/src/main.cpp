#include <Common/Macro.h>

// Enabling any of these will cause logging of certain sections //

#define LEXER_SPLITTER_LOGGING

#include <Lexer/inc/Lexer.h>
#include <Lexer/inc/File.h>

int main()
{
	try
	{
		std::string file = ReadFile("D:/repos/LX-Compiler/Tester/Examples/Test.lx");
		Lexer l(file);
	}

	catch (std::exception& e)
	{
		std::cout << e.what() << "\n\n";
		return -1;
	}
}