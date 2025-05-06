#pragma once

#include <LX-Common.h>

#include <Lexer.h>

namespace LX
{
	// Error type with index and character to alert the user that LX does not understand that symbol //
	struct InvalidCharInSource : public RuntimeError
	{
		GENERATE_LX_ERROR_REQUIRED_FUNCTION_DECLARATIONS;

		InvalidCharInSource(std::streamsize _col, std::streamsize _line, std::streamsize _index, char _invalid);

		static std::string* s_Source;
		static std::filesystem::path* s_SourceFile;

		std::streamsize col;
		std::streamsize line;
		std::streamsize index;

		char invalid;
	};
}
