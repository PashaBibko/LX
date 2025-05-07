#pragma once

#include <LX-Common.h>

#include <LexerInfo.h>

namespace LX
{
	// Error type with index and character to alert the user that LX does not understand that symbol //
	struct InvalidCharInSource : public RuntimeError
	{
		GENERATE_LX_ERROR_REQUIRED_FUNCTION_DECLARATIONS;

		InvalidCharInSource(const LexerInfo& info, const std::string _file);

		std::string lineContents;
		std::string file;

		std::streamsize col;
		std::streamsize line;

		char invalid;
	};
}
