#include <Parser.h>

#include <LLVM.h>
#include <Util.h>

namespace LX::AST
{
	llvm::Value* NumberLiteral::GenIR(llvm::LLVMContext& context, llvm::Module& module, llvm::IRBuilder<>& builder)
	{
		// Converts the string to it's int equivalent
		// Will eventually need to do floating point stuff here as well
		int number = std::stoi(m_Number);

		// Returns it as a llvm value (if valid)
		llvm::Value* out = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), number, true);
		ThrowIf<IRGenerationError>(out == nullptr);
		return out;
	}

	llvm::Value* Operation::GenIR(llvm::LLVMContext& context, llvm::Module& module, llvm::IRBuilder<>& builder)
	{
		// Gets the IR for both sides of the operation
		llvm::Value* lhs = m_Lhs->GenIR(context, module, builder);
		llvm::Value* rhs = m_Rhs->GenIR(context, module, builder);

		// If either side is null then return null to prevent invalid IR //
		if (lhs == nullptr || rhs == nullptr)
		{
			ThrowIf<IRGenerationError>(true);
			return nullptr;
		}

		// Will eventually get the correct operator but for now everything is add
		llvm::Value* out = builder.CreateAdd(lhs, rhs);
		ThrowIf<IRGenerationError>(out == nullptr);
		return out;
	}

	llvm::Value* ReturnStatement::GenIR(llvm::LLVMContext& context, llvm::Module& module, llvm::IRBuilder<>& builder)
	{
		if (m_Val == nullptr)
		{
			ThrowIf<IRGenerationError>(true);
			return nullptr;
		}

		else
		{
			llvm::Value* out = builder.CreateRet(m_Val->GenIR(context, module, builder));
			ThrowIf<IRGenerationError>(out == nullptr);
			return out;
		}
	}
}
