#pragma once

// Lexer foward declares fstream components so we can use them here //
#include <Lexer.h>

#include <unordered_set>
#include <memory>

// Foward declares STD stuff that is passed around //
namespace std::filesystem { class path; }

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
			// "Nodes" //

			MULTI_NODE,

			// General Nodes //

			NUMBER_LITERAL,
			OPERATION,

			// Variable manipulation //

			VARIABLE_DECLARATION,
			VARIABLE_ASSIGNMENT,
			VARIABLE_ACCESS,

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

		// Function to log the node to a file //
		virtual void Log(std::ofstream* log, unsigned depth) = 0;

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

	// Thrown if there was an unexpected (incorrect) token //
	struct UnexpectedToken
	{
		// The token type that should be there //
		Token::TokenType expected;

		// If there are multiple expected types there is an option for a custom message //
		std::string override;

		// What token was actually at that position //
		// Stored as Token not TokenType to store the location of it within the source //
		Token got;
	};

	class Scope
	{
		public:
			// Error thrown if the user tried to create a variable that already existed //
			struct __declspec(novtable) VariableAlreadyExists final {};

			// Error thrown if user tries to access variable that does not exist //
			struct __declspec(novtable) VariableDoesntExist final {};

			// Default constructor //
			Scope()
				: m_LocalVariables{}, m_Child(nullptr)
			{}

			// Gets a variable from the scope by it's name //
			bool DoesVarExist(const std::string& name);

			// Creates a variable of the given name //
			void CreateVar(const std::string& name);

		private:
			// Holds all the variables in the scope (excluding ones owned by the children //
			std::unordered_set<std::string> m_LocalVariables;

			// Holds a section of the scope, for example the variables created in a while loop //
			std::unique_ptr<Scope> m_Child;
	};

	// Holds all needed info about a function //
	// Currently only holds the body but in the future will hold: params, namespace/class-member //
	struct FunctionDefinition
	{
		// Defualt constructor (none other given) //
		FunctionDefinition();

		// The name of the function //
		std::string name;

		// The scope off the function //
		Scope scope;
		
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
	void GenerateIR(FileAST& ast, const std::string& name, const std::filesystem::path& IRPath);
}
