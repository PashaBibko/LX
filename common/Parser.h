#pragma once

// Lexer foward declares fstream components so we can use them here //
#include <Lexer.h>

#include <memory>

// Foward declares all items of the llvm lib that we need //
// Done to avoid including LLVM.h to shorten compile times //
namespace llvm
{
	class Value;
	class LLVMContext;
	class Module;

	class ConstantFolder;
	class IRBuilderDefaultInserter;

	template<typename T1 = ConstantFolder, typename T2 = IRBuilderDefaultInserter>
	class IRBuilder;
}

// The nodes of the abstract syntax tree constructed by the parser from the tokens //
namespace LX::AST
{
	// Base node that everything else inherits from
	class Node
	{
		public:
			// Enum for storing the type of node //
			// Used so a pointer to Node can be used and then turned into it's true type //
			enum NodeType
			{
				// General Nodes //

				IDENTIFIER,

				// Control flow Nodes //

				// If an error happened somewhere //
				UNDEFINED = -1
			};

			// Constructor to set the node type //
			Node(NodeType type)
				: m_Type(type)
			{}

			// Virtual destructor because of polymorphism //
			virtual ~Node() = default;

			// Function for generating LLVN IR (Intermediate representation) //
			virtual llvm::Value* GenIR(llvm::LLVMContext& context, llvm::Module& module, llvm::IRBuilder<>& builder) = 0;

			// Function for generating C/C++ code (Currently not implemented) //
			virtual void GenC() = 0;

			// The type of the node //
			const NodeType m_Type;
	};
}

namespace LX
{
	struct FunctionDeclaration
	{
		std::vector<std::unique_ptr<AST::Node>> body;
	};

	struct FileAST
	{
		std::vector<FunctionDeclaration> functions;
	};

	FileAST TurnTokensIntoAbstractSyntaxTree(std::vector<Token>& tokens, std::ofstream* log);
}
