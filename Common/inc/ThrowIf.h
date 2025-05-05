namespace LX
{
	// Foward declares LX::RuntimeError so it can be used to see if a class derives from //
	struct RuntimeError;

	// Util function to throw an error if the condition is met //
	// Given error type must derive from LX::RuntimeError //
	template<typename Error, typename... Args>
	inline void ThrowIf(const bool condition, Args&&... args)
	{
		// Checks that the error type will be caught by the error checker //
		static_assert
		(
			std::is_base_of_v<LX::RuntimeError, Error>,
			"Must throw a type that derives from LX::RuntimeError"
		);

		// Checks if the condition is met and micro-optimises that errors will not be thrown //
		if (condition) [[unlikely]]
		{
			// Throws a COPY of the error and not itself //
			// How C++ works, no way around it //
			throw Error(std::forward<Args>(args)...);
		}
	}
}
