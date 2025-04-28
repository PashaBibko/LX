#include <Parser.h>

#include <LLVM.h>
#include <Util.h>
#include <AST.h>

namespace LX::AST
{
	// Function for genrating LLVM IR (Intermediate representation), will throw an error if called on this class //
	llvm::Value* MultiNode::GenIR(InfoLLVM & LLVM)
	{
		throw int(); // <- TODO: Make an error type
	}

	// Function for generating LLVM IR (Intermediate representation) //
	llvm::Value* NumberLiteral::GenIR(InfoLLVM& LLVM)
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
	llvm::Value* Operation::GenIR(InfoLLVM& LLVM)
	{
		// Generates the IR for both sides of the operation //
		llvm::Value* lhs = m_Lhs->GenIR(LLVM);
		llvm::Value* rhs = m_Rhs->GenIR(LLVM);

		// If either side is null then return null to prevent invalid IR //
		// TODO: Make the error actually output information //
		if (lhs == nullptr || rhs == nullptr)
		{
			ThrowIf<IRGenerationError>(true);
			return nullptr;
		}

		// Generates the IR of the operation //
		// TODO: Support other operators other than ADD //
		// TODO: Make the error actually output information //
		llvm::Value* out = LLVM.builder.CreateAdd(lhs, rhs);
		ThrowIf<IRGenerationError>(out == nullptr);
		return out;
	}

	// Function for generating LLVM IR (Intermediate representation) //
	llvm::Value* ReturnStatement::GenIR(InfoLLVM& LLVM)
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
			llvm::Value* out = LLVM.builder.CreateRet(m_Val->GenIR(LLVM));
			ThrowIf<IRGenerationError>(out == nullptr);
			return out;
		}
	}

	// Function for generating LLVM IR (Intermediate representation) //
	llvm::Value* VariableDeclaration::GenIR(InfoLLVM& LLVM)
	{
		// Creates the variable within the scope //
		return LLVM.scope->CreateVar(m_Name, LLVM);
	}

	llvm::Value* VariableAssignment::GenIR(InfoLLVM& LLVM)
	{
		// Gets the variable from the current scope //
		llvm::AllocaInst* asignee = LLVM.scope->GetVar(m_Name);
		ThrowIf<Scope::VariableDoesntExist>(asignee == nullptr);

		// Creates the assignment //
		return LLVM.builder.CreateStore(m_Value->GenIR(LLVM), asignee);
	}

	llvm::Value* VariableAccess::GenIR(InfoLLVM& LLVM)
	{
		return nullptr;
	}
}
