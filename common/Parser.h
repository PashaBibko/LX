#pragma once

// Lexer foward declares fstream components so we can use them here //
#include <Lexer.h>

#include <memory>

// Foward declares all items of the llvm lib that we need //
// Done to avoid including LLVM.h to shorten compile times //
namespace llvm { class Value; }

// Foward declares the wrapper around the LLVM objects we need to pass around // 
namespace LX { struct InfoLLVM; }

// The nodes of the abstract syntax tree constructed by the parser from the tokens //
namespace LX::AST
{
	// Base node that everything else inherits from
	struct Node
	{
		// Enum for storing the type of node //
		// Used so a pointer to Node can be used and then turned into it's true type //
		enum NodeType
		{
			// General Nodes //

			IDENTIFIER,
			NUMBER_LITERAL,
			OPERATION,

			// Control flow Nodes //

			RETURN_STATEMENT,

			// If an error happened somewhere //
			UNDEFINED = -1
		};

		// Constructor to set the node type (no others provided) //
		Node(NodeType type);

		// Virtual destructor because of polymorphism //
		virtual ~Node() = default;

		// Function for generating LLVN IR (Intermediate representation) //
		virtual llvm::Value* GenIR(InfoLLVM& LLVM) = 0;

		// Function for generating C/C++ code (Currently not implemented) //
		//virtual void GenC() = 0;

		// The type of the node //
		const NodeType m_Type;
	};
}

namespace LX
{
	// Thrown if there was an error during IR Generation //
	struct IRGenerationError {};

	// Holds all needed info about a function //
	// Currently only holds the body but in the future will hold: name, params, namespace/class-member
	struct FunctionDefinition
	{
		// Defualt constructor (none other given) //
		FunctionDefinition();
		
		// The instructions of the body of the function //
		std::vector<std::unique_ptr<AST::Node>> body;
	};

	struct FileAST
	{
		// Default constructor (none other given) //
		FileAST();

		// All the functions within this file //
		std::vector<FunctionDefinition> functions;
	};

	// Turns the tokens of a file into it's abstract syntax tree equivalent //
	FileAST TurnTokensIntoAbstractSyntaxTree(std::vector<Token>& tokens, std::ofstream* log);

	// Turns an abstract binary tree into LLVM intermediate representation //
	void GenerateIR(FileAST& ast);
}
