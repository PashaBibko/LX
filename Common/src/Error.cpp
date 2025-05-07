#include <LX-Common.h>

namespace LX
{
	RuntimeError::RuntimeError()
	{
		// Debuggers can only be attached in Debug configuration so this code is useless in Release/Distribution builds //
		#ifdef _DEBUG

		// Checks a debugger is present before throwing a breakpoint //
		if (IsDebuggerPresent()) { __debugbreak(); }

		#endif
	}

	InvalidFilePath::InvalidFilePath(const std::string& _name, const std::filesystem::path& path)
		: name(nullptr), fileLocation(nullptr)
	{
		// Stores the actual strings in static memory //
		
		static std::string s_ErrorPath;
		static std::string s_ErrorName;

		s_ErrorPath = path.string();
		s_ErrorName = _name;

		// Assigns a pointer to the c-string of the strings //
		// Done like this because of how DLLs work //

		fileLocation = s_ErrorPath.c_str();
		name = s_ErrorName.c_str();
	}

	void InvalidFilePath::PrintToConsole() const
	{
		// Tells the user the input file could not be found and how to fix the issue //
		LX::PrintAsColor<Color::LIGHT_RED>("Error: ");
		std::cout << "Invalid " << name << ": ";
		LX::PrintAsColor<Color::WHITE>(fileLocation);
		std::cout << "\n\nMake sure the file exists and the process has the correct path to the file\n";
	}

	const char* InvalidFilePath::ErrorType() const
	{
		return "Invalid File Path";
	}
}
