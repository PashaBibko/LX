#include <LX-Common.h>

#include <Parser.h>

#include <ParserErrors.h>
#include <Scope.h>

namespace LX::AST
{
	// Function for genrating LLVM IR (Intermediate representation), will throw an error if called on this class //
	llvm::Value* MultiNode::GenIR(InfoLLVM& LLVM, FunctionScope& func)
	{
		throw int(); // <- TODO: Make an error type
	}

	// Function for generating LLVM IR (Intermediate representation) //
	llvm::Value* NumberLiteral::GenIR(InfoLLVM& LLVM, FunctionScope& func)
	{
		// Converts the string to it's int equivalent //
		// TODO: Support floating point values //
		int number = std::stoi(m_Number);

		// Returns it as a llvm value (if valid) //
		// TODO: Support floating point values //
		// TODO: Make the error actually output information //
		llvm::Value* out = llvm::ConstantInt::get(llvm::Type::getInt32Ty(LLVM.context), number, true);
		ThrowIf<IRGenerationError>(out == nullptr);
		return out;
	}

	// Function for generating LLVM IR (Intermediate representation) //
	llvm::Value* Operation::GenIR(InfoLLVM& LLVM, FunctionScope& func)
	{
		// Generates the IR for both sides of the operation //
		llvm::Value* lhs = m_Lhs->GenIR(LLVM, func);
		llvm::Value* rhs = m_Rhs->GenIR(LLVM, func);

		// If either side is null then return null to prevent invalid IR //
		// TODO: Make the error actually output information //
		if (lhs == nullptr || rhs == nullptr)
		{
			ThrowIf<IRGenerationError>(true);
			return nullptr;
		}

		// Generates the IR of the operation //
		llvm::Value* out = nullptr;

		// Creates the correct operation depending on the operand //
		switch (m_Operand)
		{
			case Token::ADD:
				out = LLVM.builder.CreateAdd(lhs, rhs);
				break;

			case Token::SUB:
				out = LLVM.builder.CreateSub(lhs, rhs);
				break;

			case Token::MUL:
				out = LLVM.builder.CreateMul(lhs, rhs);
				break;

			case Token::DIV:
				out = LLVM.builder.CreateSDiv(lhs, rhs);
				break;

			default:
				// TODO: Add an error here
				out = nullptr;
		}
		
		// Checks it all went succesfully before returning //
		ThrowIf<IRGenerationError>(out == nullptr);
		return out;
	}

	// Function for generating LLVM IR (Intermediate representation) //
	llvm::Value* ReturnStatement::GenIR(InfoLLVM& LLVM, FunctionScope& func)
	{
		// Checks if it is a void return //
		if (m_Val == nullptr)
		{
			// Void returns are currently not implemented //
			// TODO: Find out how to return nothing from a function //
			ThrowIf<IRGenerationError>(true);
			return nullptr;
		}

		// Else it will be returning a value //
		else
		{
			// Generates the value and creates a return for it //
			// TODO: Make the error actually output information //
			llvm::Value* out = LLVM.builder.CreateRet(m_Val->GenIR(LLVM, func));
			ThrowIf<IRGenerationError>(out == nullptr);
			return out;
		}
	}

	// Function for generating LLVM IR (Intermediate representation) //
	llvm::Value* VariableDeclaration::GenIR(InfoLLVM& LLVM, FunctionScope& func)
	{
		return func.DecVar(m_Name, LLVM);
	}

	llvm::Value* VariableAssignment::GenIR(InfoLLVM& LLVM, FunctionScope& func)
	{
		return func.AssignVar(m_Name, m_Value.get(), LLVM, func);
	}

	llvm::Value* VariableAccess::GenIR(InfoLLVM& LLVM, FunctionScope& func)
	{
		return func.AccessVar(m_Name, LLVM);
	}
	
	llvm::Value* FunctionCall::GenIR(InfoLLVM& LLVM, FunctionScope& func)
	{
		std::vector<llvm::Value*> evaluatedArgs;

		for (std::unique_ptr<Node>& node : m_Args)
		{
			evaluatedArgs.push_back(node->GenIR(LLVM, func));
		}

		return LLVM.builder.CreateCall(LLVM.functions[m_Name], evaluatedArgs, "call_tmp");
	}
}
