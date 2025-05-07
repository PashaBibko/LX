namespace LX
{
	// Base error class for all LX thrown errors //
	// Holds nothing apart from the v-table //
	struct COMMON_API RuntimeError
	{
		// Default constructor which throws a breakpoint on being created //
		RuntimeError();

		// Prints the error to the console //
		// Include Common/Console.h for printing util functions //
		virtual void PrintToConsole() const = 0;

		// Returns a C-String of the type that was thrown //
		virtual const char* ErrorType() const = 0;

		// Virtual destructor because of polymorphism //
		virtual ~RuntimeError() = default;
	};

	// --- Common errors that can be thrown --- //

	// Error thrown when there is an invalid file path //
	struct COMMON_API InvalidFilePath : public RuntimeError
	{
		// Constructor to turn the C++ types to C to expose them in DLL //
		InvalidFilePath(const std::string& _name, const std::filesystem::path& path);

		// Prints the error to the console //
		void PrintToConsole() const;
		
		// Returns the error as c-string //
		const char* ErrorType() const;

		// Name of the file that is invalid (used for console output) //
		const char* name;

		// The location of the file (used for console output) //
		const char* fileLocation;
	};
}

// Helper macro to autogenerate a basic error type in a .h file //
// Still requires function definitions in a .cpp file //
#define CREATE_EMPTY_LX_ERROR_TYPE(name)\
struct name : public LX::RuntimeError{ GENERATE_LX_ERROR_REQUIRED_FUNCTION_DECLARATIONS };

// Helper macro to autogenerate function declarations of functions required by LX::RuntimeError //
#define GENERATE_LX_ERROR_REQUIRED_FUNCTION_DECLARATIONS void PrintToConsole() const; const char* ErrorType() const;
