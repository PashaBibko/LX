namespace LX
{
	// Static class used for logging information //
	class COMMON_API Log
	{
		public:
			// This class should never be constructed //
			// It acts like a fancy namespace //
			Log() = delete;

			enum class Format
			{
				AUTO,
				NONE
			};

			template<Format format = Format::AUTO, typename... Args>
			static void out(Args... args)
			{
				// Prints out the args ending with a new line unless specified //
				if constexpr (format == Format::AUTO) { ((*s_LogFile << ... << args) << "\n"); }

				// Else prints out the args as provided //
				else { (*s_LogFile << ... << args); }
			}

			template<typename... Args>
			static void LogNewSection(Args... args)
			{
				static const char* BREAK = "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-";

				*s_LogFile << '\n' << BREAK << '\n';
				(*s_LogFile << ... << args);
				*s_LogFile << '\n' << BREAK << '\n';
			}

			// Initalises the log (Called by DllMain) //
			static void Init();

		private:
			// Keeps the pointer hidden to stop accidental changes //
			static std::ofstream* s_LogFile;
	};
}
