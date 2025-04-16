#pragma once

#include <fstream>

namespace LX
{
	template<typename T, typename... Args>
	inline void ThrowIf(const bool condition, Args... args)
	{ if (condition) [[unlikely]] { throw T(args...); }}

	template<typename... Args>
	inline void SafeLog(std::ofstream* log, Args... args)
	{
		if (log != nullptr) { (*log << ... << args); *log << "\n"; }
	}

	constexpr const char* LOG_BREAK = "\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n";
}
