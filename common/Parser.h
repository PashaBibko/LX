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

		// Constructor to set the node type //
		Node(NodeType type)
			: m_Type(type)
		{}

		// Virtual destructor because of polymorphism //
		virtual ~Node() = default;

		// Function for generating LLVN IR (Intermediate representation) //
		virtual llvm::Value* GenIR(llvm::LLVMContext& context, llvm::Module& module, llvm::IRBuilder<>& builder) = 0;

		// Function for generating C/C++ code (Currently not implemented) //
		//virtual void GenC() = 0;

		// The type of the node //
		const NodeType m_Type;
	};

	class NumberLiteral : public Node
	{
		public:
			NumberLiteral(std::string num)
				: Node(Node::NUMBER_LITERAL), m_Number(num)
			{}

			llvm::Value* GenIR(llvm::LLVMContext& context, llvm::Module& module, llvm::IRBuilder<>& builder)
			{
				return nullptr;
			}

		private:
			// The number it stores
			// Yes the number is stored as a string
			// It's horrible I know
			std::string m_Number;
	};

	//
	class Operation : public Node
	{
		public:
			Operation(std::unique_ptr<Node> lhs, Token::TokenType op, std::unique_ptr<Node> rhs)
				: Node(Node::OPERATION), m_Lhs(std::move(lhs)), m_Operand(op), m_Rhs(std::move(rhs))
			{}

			llvm::Value* GenIR(llvm::LLVMContext& context, llvm::Module& module, llvm::IRBuilder<>& builder)
			{
				return nullptr;
			}

		private:
			// The sides of the operation
			// Unary operations are handled by a different class
			std::unique_ptr<Node> m_Lhs, m_Rhs;

			// The operation to be applied to the two sides
			Token::TokenType m_Operand;
	};

	//
	class ReturnStatement : public Node
	{
		public:
			ReturnStatement(std::unique_ptr<Node> val)
				: Node(Node::RETURN_STATEMENT), m_Val(std::move(val))
			{}

			llvm::Value* GenIR(llvm::LLVMContext& context, llvm::Module& module, llvm::IRBuilder<>& builder)
			{
				return nullptr;
			}

		private:
			// What it is returning (can be null)
			std::unique_ptr<Node> m_Val;
	};
}

namespace LX
{
	struct FunctionDefinition
	{
		FunctionDefinition()
			: body{}
		{}
		
		std::vector<std::unique_ptr<AST::Node>> body;
	};

	struct FileAST
	{
		FileAST()
			: functions{}
		{}

		std::vector<FunctionDefinition> functions;
	};

	FileAST TurnTokensIntoAbstractSyntaxTree(std::vector<Token>& tokens, std::ofstream* log);
}
