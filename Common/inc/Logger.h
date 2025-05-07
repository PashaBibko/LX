namespace LX
{
	// Static class used for logging information //
	class COMMON_API Log
	{
		public:
			// This class should never be constructed //
			// It acts like a fancy namespace //
			Log() = delete;

			// The different formats the log supports //
			enum class Format
			{
				// Default, with a new line character added after every call //
				AUTO,

				// No new line characters after all the provided arguments //
				NONE
			};

			// Variadic template to allow an undefined ammount of arguments //
			template<Format format = Format::AUTO, typename... Args>
				requires AllLogable<Args...> // <- Checks all types can be outputted to the console

			// Logs information (if the log is initalised) //
			static void out(Args... args)
			{
				// Prints out the args ending with a new line unless specified //
				if constexpr (format == Format::AUTO) { ((*s_LogFile << ... << args) << "\n"); }

				// Else prints out the args as provided //
				else { (*s_LogFile << ... << args); }
			}

			// Variadic template to allow an undefined ammount of arguments //
			template<typename... Args>
				requires AllLogable<Args...> // <- Checks all types can be outputted to the console

			// Adds a named break between different sections in the log //
			static void LogNewSection(Args... args)
			{
				// Constant for how a break is represented in the log //
				static const char* BREAK = "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-";

				// Outputs the arguments between two breaks //
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
