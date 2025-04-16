#pragma once

namespace LX
{
	template<typename T, typename... Args>
	inline void ThrowIf(const bool condition, Args... args)
	{ if (condition) [[unlikely]] { throw T(args...); }}
}
