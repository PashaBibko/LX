#include <Common/Macro.h>

#include <Lexer/inc/Lexer.h>
#include <Parser/inc/AST.h>

#include <iostream>

int main()
{
	try
	{
		std::string file = LX::ReadFile("D:/repos/LX-Compiler/Tester/Examples/Test.lx");
		LX::Lexer l(file);
	}

	catch (std::exception& e)
	{
		std::cout << e.what() << "\n\n";
		return -1;
	}
}