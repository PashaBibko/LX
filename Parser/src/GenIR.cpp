#include <Parser.h>
#include <Util.h>

#include <iostream>

namespace LX
{
	static constexpr bool IsValidTopLevelNode(AST::Node::NodeType type)
	{
		return true;
	}

	static void GenerateFunctionIR(FunctionDefinition& funcAST, llvm::LLVMContext& context, llvm::Module& module, llvm::IRBuilder<>& builder)
	{
		// Creates the functions signature and return type //

		llvm::FunctionType* retType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false); // <- Defaults to int currently
		llvm::Function* func = llvm::Function::Create(retType, llvm::Function::ExternalLinkage, "main", module); // Defaults to main currently
		llvm::BasicBlock* entry = llvm::BasicBlock::Create(context, "entry", func);
		builder.SetInsertPoint(entry);

		// Generates the IR within the function //

		for (auto& node : funcAST.body)
		{
			ThrowIf<int>(IsValidTopLevelNode(node->m_Type) == false); // <- TODO: replace with actual error type
			node->GenIR(context, module, builder);
		}

		// Adds a terminator if there is none //

		if (entry->getTerminator() == nullptr)
		{
			builder.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0, true));
		}

		// Verifies the function works //

		ThrowIf<int>(llvm::verifyFunction(*func), &llvm::errs()); // <- Make error type
	}

	void GenerateIR(FileAST& ast)
	{
		// Creates the LLVM variables needed for generating IR that are shared between functions //

		llvm::LLVMContext context;
		llvm::IRBuilder<> builder(context);
		llvm::Module module("add_ints", context);

		// Loops over AST to generate IR //

		for (auto& func : ast.functions)
		{
			GenerateFunctionIR(func, context, module, builder);
		}

		// Outputs the IR to the console //

		module.print(llvm::outs(), nullptr);

		std::cout << "Finished generating IR" << std::endl;
	}
}
