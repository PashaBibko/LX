#include <LX-Common.h>

#include <Scope.h>

#include <AST.h>

namespace LX
{
	const char* VariableError::ErrorType() const
	{
		return "Variable Error";
	}

	void VariableError::PrintToConsole() const
	{
	}
}
