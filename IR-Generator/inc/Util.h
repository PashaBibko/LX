#pragma once

#include <fstream>

// Defining this is only if you are at the point where you should be using a debugger //
#define LOG_EVERYTHING

namespace LX
{
	template<typename T, typename... Args>
	// Helper function to throw given error if condition is true //
	// Also micro-optimises to predict there is no errors thrown //
	inline void ThrowIf(const bool condition, Args... args)
	{ if (condition) [[unlikely]] { throw T(args...); }}

	template<typename... Args>
	// Helper function for logging //
	// Only logs the given args if the log is not null //
	inline void SafeLog(std::ofstream* log, Args... args)
	{
		if (log != nullptr) { (*log << ... << args); *log << "\n"; }
	}

	inline void SafeFlush(std::ofstream* log)
	{
		if (log != nullptr)
		{
			log->flush();
		}
	}

	// Gives a standard way to mark a change between different sections within the log output //
	constexpr const char* LOG_BREAK = "\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
}
