#include <UnitTester/inc/UnitTester.h>

#include <iostream>

UnitTestsOutput::UnitTest::UnitTest(const std::string& name, bool passed)
	: name(name), passed(passed)
{}

UnitTestsOutput::UnitTestsOutput()
	: m_AllTestsPassed(true)
{
	// Starts the clock
	m_Start = std::chrono::high_resolution_clock::now();
}

void UnitTestsOutput::Push(const UnitTest& newTest)
{
	// Updates m_AllTestsPassed if neccesary
	if (newTest.passed == false)
	{
		m_AllTestsPassed = false;
	}

	// Adds the test to the vector
	m_Tests.push_back(newTest);
}

void UnitTestsOutput::Display()
{
	if (m_AllTestsPassed == true)
	{
		// Calculates time taken
		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_Start);

		// Displays time taken and test count
		std::cout << "All tests passed\n";
		std::cout << " - Test count: " << m_Tests.size() << "\n";
		std::cout << " - Time taken: " << duration.count() / 1000.0f << "s\n";
	}

	// Tells me I messed up somewhere
	else
	{
		std::cout << "Tests failed:\n";

		// Displays all tests that failed
		for (const UnitTest& test : m_Tests)
		{
			if (test.passed == false)
			{
				std::cout << " - " << test.name << "\n";
			}
		}
	}

	// Flushes the console to make sure all messages have been displayed
	std::cout << std::flush;
}