namespace LX
{
	class COMMON_API Log
	{
		public:
			// This class should never be constructed //
			Log() = delete;

			enum class Format
			{
				AUTO,
				NONE
			};

			template<Format format = Format::AUTO, typename... Args>
			static void out(Args... args)
			{
				if constexpr (format == Format::AUTO)
				{
					((*s_LogFile << ... << args) << "\n");
				}

				else
				{
					(*s_LogFile << ... << args);
				}
			}

			template<typename... Args>
			static void LogNewSection(Args... args)
			{
				static const char* BREAK = "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-";

				*s_LogFile << '\n' << BREAK << '\n';
				(*s_LogFile << ... << args);
				*s_LogFile << '\n' << BREAK << '\n';
			}

		private:
			// Allows ProcAttach to manage the log //
			friend bool ProcAttach(HMODULE hModule);

			// Initalises the log (Called by DllMain) //
			static void Init();

			// Closes the log (Called by DllMain) //
			static void Close();

			static std::ofstream* s_LogFile;
	};
}
