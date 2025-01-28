#pragma once

#include <string>
#include <vector>
#include <chrono>

// Simple class to keep track of if tests passed
class UnitTestsOutput
{
	public:
		// Struct to hold name of test and wether it passed
		struct UnitTest
		{
			UnitTest(const std::string& name, bool passed);

			const std::string name;
			const bool passed;
		};

		// Starts the clock
		UnitTestsOutput();

		// Adds a new test to the vector
		void Push(const UnitTest& newTest);

		// Displays wether the tests passed or not in the console
		void Display();

	private:
		// Time at which the unit tests started
		std::chrono::high_resolution_clock::time_point m_Start;

		// All of the tests that have been added
		std::vector<UnitTest> m_Tests;

		// Wether all of the tests have passed to avoid checking on display()
		bool m_AllTestsPassed;
};