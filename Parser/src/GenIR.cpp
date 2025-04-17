#include <Parser.h>

#include <iostream>

namespace LX
{
	void GenerateIR(FileAST& ast)
	{
		// Generates stuff //

		llvm::LLVMContext context;
		llvm::IRBuilder<> builder(context);
		
		{
			std::unique_ptr<llvm::Module> module = std::make_unique<llvm::Module>("add_ints", context);

			// Defines main function //

			llvm::FunctionType* funcType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false);
			llvm::Function* mainFunc = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module.get());
			llvm::BasicBlock* entry = llvm::BasicBlock::Create(context, "entry", mainFunc);
			builder.SetInsertPoint(entry);

			// Loops over AST to generate IR //

			for (auto& node : ast.functions[0].body)
			{
				switch (node->m_Type)
				{
					case AST::Node::RETURN_STATEMENT:
					{
						node->GenIR(context, *module, builder);
						break;
					}

					default:
					{
						break;
					}
				}
			}

			if (entry->getTerminator() == nullptr)
			{
				builder.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0, true));
			}

			// Verification of the IR //

			if (llvm::verifyFunction(*mainFunc, &llvm::errs()) || llvm::verifyModule(*module, &llvm::errs()))
			{
				std::cerr << "Error: IR generation failed" << std::endl;
				return;
			}

			// Outputs the IR to the console //

			module->print(llvm::outs(), nullptr);

		} // <- Crashes here

		std::cout << "Finished generating IR" << std::endl;
	}
}
