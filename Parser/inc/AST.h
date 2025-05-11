#pragma once

#include <LX-Common.h>

#include <Parser.h>

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

		// All IR functions that have been generated //
		std::unordered_map<std::string, llvm::Function*> functions;
	};
}

namespace LX::AST
{
	class MultiNode : public Node
	{
		public:
			// Constructor to auto set type //
			MultiNode();

			// Function for generating LLVM IR (Intermediate representation), will throw error if called on this class //
			llvm::Value* GenIR(InfoLLVM& LLVM, FunctionScope& func) override;

			// Function to log the node to a file, will throw an error if called on this class //
			void Log(unsigned depth) override;

			// Function to get the node's type name, will throw an error if called on this class //
			virtual const char* TypeName() override;

			// The nodes that are contained within this node //
			std::vector<std::unique_ptr<Node>> nodes;
	};

	// Node to represent any number within the AST //
	class NumberLiteral : public Node
	{
		public:
			// Constructor to set values and automatically set type //
			NumberLiteral(std::string num);

			// Function for generating LLVM IR (Intermediate representation) //
			llvm::Value* GenIR(InfoLLVM& LLVM, FunctionScope& func) override;

			// Function to log the node to a file //
			void Log(unsigned depth) override;

			// Function to get the node's type name //
			const char* TypeName() override;

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

			// Function for generating LLVM IR (Intermediate representation) //
			llvm::Value* GenIR(InfoLLVM& LLVM, FunctionScope& func) override;

			// Function to log the node to a file //
			void Log(unsigned depth) override;

			// Function to get the node's type name //
			const char* TypeName() override;

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

			// Function for generating LLVM IR (Intermediate representation) //
			llvm::Value* GenIR(InfoLLVM& LLVM, FunctionScope& func) override;

			// Function to log the node to a file //
			void Log(unsigned depth) override;

			// Function to get the node's type name //
			const char* TypeName() override;

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

			// Function for generating LLVM IR (Intermediate representation) //
			llvm::Value* GenIR(InfoLLVM& LLVM, FunctionScope& func) override;

			// Function to log the node to a file //
			void Log(unsigned depth) override;

			// Function to get the node's type name //
			const char* TypeName() override;

		private:
			// Name of the variable //
			std::string m_Name;

			// Doesnt need to store type as everything is currently int //
	};

	// Node to represent the assignment of a variable within the AST //
	class VariableAssignment : public Node
	{
		public:
			// Constructor to set values and automatically set type //
			VariableAssignment(const std::string& name, std::unique_ptr<AST::Node> val);

			// Function for generating LLVM IR (Intermediate representation) //
			llvm::Value* GenIR(InfoLLVM& LLVM, FunctionScope& func) override;

			// Function to log the node to a file //
			void Log(unsigned depth) override;

			// Function to get the node's type name //
			const char* TypeName() override;

		private:
			// Name of the variable //
			std::string m_Name;

			// The value that will be assigned to the value //
			std::unique_ptr<Node> m_Value;
	};

	// Node to represent accessing a variable within the AST //
	class VariableAccess : public Node
	{
		public:
			// Constructor to set values and automatically set type //
			VariableAccess(const std::string& name);

			// Function for generating LLVM IR (Intermediate representation) //
			llvm::Value* GenIR(InfoLLVM& LLVM, FunctionScope& func) override;

			// Function to log the node to a file //
			void Log(unsigned depth) override;

			// Function to get the node's type name //
			const char* TypeName() override;

		private:
			// The name of the variable //
			std::string m_Name;
	};

	// Node to represent calling a function within the AST //
	class FunctionCall : public Node
	{
		public:
			// Constructor to set the name of the function and any args it may have //
			FunctionCall(const std::string& funcName, std::vector<std::unique_ptr<Node>>& args);
			
			// Function for generating LLVM IR (Intermediate representation) //
			llvm::Value* GenIR(InfoLLVM& LLVM, FunctionScope& func) override;

			// Function to log the niode to a file //
			void Log(unsigned depth) override;

			// Function to get the node's type name //
			const char* TypeName() override;

		private:
			// The name of the function //
			std::string m_Name;

			// Any arguments to pass into the function //
			std::vector<std::unique_ptr<Node>> m_Args;
	};
}