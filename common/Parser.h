#pragma once

#include <Lexer.h>
#include <LLVM.h>

#include <fstream>
#include <vector>
#include <memory>

namespace LX
{
	struct GlobalDeclaration
	{
	};

	struct FunctionDeclaration
	{
	};

	std::vector<std::unique_ptr<GlobalDeclaration>> TurnTokensIntoAbstractSyntaxTree(std::vector<Token>& tokens, std::ofstream* log);
}
