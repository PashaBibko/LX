namespace LX
{
	template<typename... Args>
	inline void SafeLog(std::ofstream* log, Args... args)
	{
		if (log != nullptr)
		{
			(*log << ... << args);
			(*log << '\n');
		}
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
