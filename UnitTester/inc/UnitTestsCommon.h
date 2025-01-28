#pragma once

#include <iostream>
#include <vector>

class UnitTestsOutput
{
	public:
		struct UnitTest
		{
			UnitTest(std::string name, const bool passed)
				: name(name), passed(passed)
			{}

			const std::string name;
			const bool passed;
		};

		UnitTestsOutput()
			: m_AllTestsPassed(true)
		{}

		void Push(const UnitTest& newTest)
		{
			if (newTest.passed == false)
			{
				m_AllTestsPassed = false;
			}

			m_Tests.push_back(newTest);
		}

		void Display()
		{
			if (m_AllTestsPassed == true)
			{
				std::cout << "All tests passed\n";
			}

			else
			{
				std::cout << "Tests failed:\n";

				for (const UnitTest& test : m_Tests)
				{
					if (test.passed == false)
					{
						std::cout << " - " << test.name << "\n";
					}
				}
			}

			std::cout << std::flush;
		}

	private:
		std::vector<UnitTest> m_Tests;

		bool m_AllTestsPassed;
};