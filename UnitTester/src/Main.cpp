#include <Common/inc/Token.h>

#include <UnitTester/inc/UnitTester.h>

#include <limits>

static void TestTokens(UnitTestsOutput& tests)
{
	// Text used in all tests
	std::string src = "Hello world!";

	// Simple token creation
	{
		Token t(Token::Type::UNDEFINED, src, 2, 5);

		tests.Push({ "Simple token creation", t.Str() == "llo w" });
	}

	// Catch start of token being in bounds
	{
		bool caughtCorrectError = false;

		try
		{
			Token t(Token::Type::UNDEFINED, src, 99, 1);
		}

		catch (std::runtime_error& e)
		{
			if (std::string(e.what()) != "Start of token is outside string bounds")
			{
				throw e;
			}

			else
			{
				caughtCorrectError = true;
			}
		}

		tests.Push({ "Catch start of token outside string bounds", caughtCorrectError });
	}

	// Catch token length being in bounds
	{
		bool caughtCorrectError = false;

		try
		{
			Token t(Token::Type::UNDEFINED, src, 0, 99);
		}

		catch (std::runtime_error& e)
		{
			if (std::string(e.what()) != "Token length overflows given string")
			{
				throw e;
			}

			else
			{
				caughtCorrectError = true;
			}
		}

		tests.Push({ "Catch token length overflowing string", caughtCorrectError });
	}
}

int main()
{
	// Data type to keep track of test output
	UnitTestsOutput tests;

	try
	{
		// The testing

		TestTokens(tests);
	}

	// Catches all accidental runtime errors
	catch (std::exception& e)
	{
		std::cout << "Error caught in tests: " << e.what() << "\n\n";
		return -1;
	}

	// Displays when all tests have passed
	// Shows ammount of tests and how long it took
	tests.Display();
}