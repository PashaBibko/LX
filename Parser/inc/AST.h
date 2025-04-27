#include <Parser.h>

#include <LLVM.h>

#include <memory>

namespace LX
{
	// Wrapper over the LLVM variables for easier passing around //
	struct InfoLLVM
	{
		// Constructor to initalize them correctly (only constructor available) //
		InfoLLVM(std::string name);

		llvm::LLVMContext context;
		llvm::Module module;
		llvm::IRBuilder<> builder;
	};
}

namespace LX::AST
{
	// Node to represent any number within the AST //
	class NumberLiteral : public Node
	{
		public:
			// Constructor to set values and automatically set type //
			NumberLiteral(std::string num);

			// Function for generating LLVN IR (Intermediate representation) //
			llvm::Value* GenIR(InfoLLVM& LLVM) override;

			// Function to log the node to a file //
			void Log(std::ofstream* log, unsigned depth) override;

		private:
			// The number it stores //
			// Yes the number is stored as a string, It's horrible I know //
			std::string m_Number;
	};

	// Node to represent any 2-sided mathematical or logical operation within the AST //
	class Operation : public Node
	{
		public:
			// Constructor to set values and automatically set type //
			Operation(std::unique_ptr<Node> lhs, Token::TokenType op, std::unique_ptr<Node> rhs);

			// Function for generating LLVN IR (Intermediate representation) //
			llvm::Value* GenIR(InfoLLVM& LLVM) override;

			// Function to log the node to a file //
			void Log(std::ofstream* log, unsigned depth) override;

		private:
			// The sides of the operation //
			// Unary operations are handled by a different class //
			std::unique_ptr<Node> m_Lhs, m_Rhs;

			// The operation to be applied to the two sides //
			Token::TokenType m_Operand;
	};

	// Node to represent any return statement within the AST //
	class ReturnStatement : public Node
	{
		public:
			// Constructor to set values and automatically set type //
			ReturnStatement(std::unique_ptr<Node> val);

			// Function for generating LLVN IR (Intermediate representation) //
			llvm::Value* GenIR(InfoLLVM& LLVM) override;

			// Function to log the node to a file //
			void Log(std::ofstream* log, unsigned depth) override;

		private:
			// What it is returning (can be null) //
			std::unique_ptr<Node> m_Val;
	};

	// Node to represent the declaration of a variable within the AST //
	class VariableDeclaration : public Node
	{
		public:
			// Constructor to set values and automatically set type //
			VariableDeclaration(const std::string& name);

			// Function for generating LLVN IR (Intermediate representation) //
			llvm::Value* GenIR(InfoLLVM& LLVM) override;

			// Function to log the node to a file //
			void Log(std::ofstream* log, unsigned depth) override;

		private:
			// Name of the variable //
			std::string m_Name;

			// Doesnt need to store type as everything is currently int //
	};
}