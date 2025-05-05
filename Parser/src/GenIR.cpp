#include <LX-Common.h>

#include <Parser.h>

#include <AST.h>

namespace LX
{
	// Tells the generator if the current node is allowed to be within a top-level context //
	// TODO: Make this function do something other than return true //
	static constexpr bool IsValidTopLevelNode(AST::Node::NodeType type)
	{
		return true;
	}

	// Generates the LLVM IR for the given function //
	static void GenerateFunctionIR(FunctionDefinition& funcAST, InfoLLVM& LLVM)
	{
		// Creates the functions signature and return type //

		llvm::FunctionType* retType = llvm::FunctionType::get(llvm::Type::getInt32Ty(LLVM.context), false); // <- Defaults to int currently
		llvm::Function* func = llvm::Function::Create(retType, llvm::Function::ExternalLinkage, funcAST.name, LLVM.module);
		llvm::BasicBlock* entry = llvm::BasicBlock::Create(LLVM.context, "entry", func);
		LLVM.builder.SetInsertPoint(entry);

		// Generates the IR within the function by looping over the nodes //
		for (auto& node : funcAST.body)
		{
			ThrowIf<IRGenerationError>(IsValidTopLevelNode(node->m_Type) == false); // <- TODO: replace with actual error type
			node->GenIR(LLVM);
		}

		// Adds a terminator if there is none //
		if (entry->getTerminator() == nullptr)
		{
			LLVM.builder.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(LLVM.context), 0, true));
		}

		// Verifies the function works //
		ThrowIf<IRGenerationError>(llvm::verifyFunction(*func)); // <- TODO: Make error type
	}

	// Turns an abstract binary tree into LLVM intermediate representation //
	void GenerateIR(FileAST& ast, const std::string& name, const std::filesystem::path& IRPath)
	{
		// Opens the file to output the IR //
		std::error_code EC;
		llvm::raw_fd_ostream file(IRPath.string(), EC);

		// Creates the LLVM variables needed for generating IR that are shared between functions //
		InfoLLVM LLVM(name);

		// Loops over the functions to generate their LLVM IR //
		for (auto& func : ast.functions)
		{
			LLVM.scope = &func.scope; // Sets the current scope for the builder
			GenerateFunctionIR(func, LLVM);
		}

		// Outputs the IR to the output file //
		LLVM.module.print(file, nullptr);
	}
}
