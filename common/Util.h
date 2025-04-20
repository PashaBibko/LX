#pragma once

#include <fstream>

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

	// Gives a standard way to mark a change between different sections within the log output //
	constexpr const char* LOG_BREAK = "\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
}
