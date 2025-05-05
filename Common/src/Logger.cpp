#include <LX-Common.h>

namespace LX
{
	// Allocates memory for the log file pointer //
	std::ofstream* Log::s_LogFile = nullptr;

	void Log::Init()
	{
		// The actual object holding the log file //
		// Has to be done like this to stop MSVC complaining //
		static std::ofstream actualLog;

		// Opens the log file and assigns it to the log pointer //
		actualLog.open("log");
		s_LogFile = &actualLog;
	}

	void Log::Close()
	{
		// Flushes and closes the log //
		// Yes I know closing automatically flushes but this function looked empty //
		s_LogFile->flush();
		s_LogFile->close();
	}
}
