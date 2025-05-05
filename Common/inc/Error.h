namespace LX
{
	// Base error class for all LX thrown errors //
	// Holds nothing apart from the v-table //
	struct RuntimeError
	{
		// Prints the error to the console //
		// Include Common/Console.h for printing util functions //
		virtual void PrintToConsole() const = 0;

		// Returns a C-String of the type that was thrown //
		virtual const char* ErrorType() const = 0;
	};
}

// Helper macro to autogenerate a basic error type in a .h file //
// Still requires function definitions in a .cpp file //
#define CREATE_EMPTY_LX_ERROR_TYPE(name)\
struct name : public LX::RuntimeError{ GENERATE_LX_ERROR_REQUIRED_FUNCTION_DECLARATIONS };

// Helper macro to autogenerate function declarations of functions required by LX::RuntimeError //
#define GENERATE_LX_ERROR_REQUIRED_FUNCTION_DECLARATIONS void PrintToConsole() const; const char* ErrorType() const;
