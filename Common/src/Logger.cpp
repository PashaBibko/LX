#include <LX-Common.h>

namespace LX
{
	// Allocates memory for the log file pointer //

	std::ofstream* Log::s_LogFile = nullptr;
	Log::Priority Log::s_Priority;

	void Log::Init(Priority _default)
	{
		// The actual object holding the log file //
		// Has to be done like this to stop MSVC complaining //
		static std::ofstream actualLog;

		// Opens the log file and assigns it to the log pointer //
		actualLog.open("log.txt");
		s_LogFile = &actualLog;

		// Assigns the priority //
		s_Priority = _default;
	}
}
