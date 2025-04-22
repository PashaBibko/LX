#include <Console.h>

#include <Windows.h>
#include <iostream>

namespace LX
{	
	void PrintStringAsColor(const std::string& str, Color c)
	{
		// Gets a handle to the console //
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		
		// Sets the color of the console to the desired color //
		SetConsoleTextAttribute(hConsole, (WORD)c);

		// Outputs the text //
		std::cout << str;

		// Resets the color //
		SetConsoleTextAttribute(hConsole, (WORD)Color::LIGHT_GRAY);
	}
}
