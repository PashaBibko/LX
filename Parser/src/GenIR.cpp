#include <LX-Common.h>

#include <Parser.h>

#include <ParserErrors.h>
#include <Scope.h>

namespace LX
{
	// Tells the generator if the current node is allowed to be within a top-level context //
	// TODO: Make this function do something other than return true //
	static constexpr bool IsValidTopLevelNode(AST::Node::NodeType type)
	{
		return true;
	}

	static llvm::GlobalValue::LinkageTypes GetLinkageType(const std::string& funcName)
	{
		if (funcName == "main")
		{
			return llvm::Function::ExternalLinkage;
		}

		else
		{
			return llvm::GlobalValue::InternalLinkage;
		}
	}

	// Generates the LLVM IR for the given function //
	static void GenerateFunctionIR(FunctionDefinition& funcAST, InfoLLVM& LLVM)
	{
		try
		{
			Log::LogNewSection("Generating ", funcAST.name, " LLVM IR");

			// Creates the functions signature and return type //

			std::vector<llvm::Type*> funcParams(funcAST.params.size(), LLVM.builder.getInt32Ty());

			llvm::FunctionType* retType = llvm::FunctionType::get(llvm::Type::getInt32Ty(LLVM.context), funcParams, false); // <- Defaults to int currently
			llvm::Function* func = llvm::Function::Create(retType, GetLinkageType(funcAST.name), funcAST.name, LLVM.module);
			llvm::BasicBlock* entry = llvm::BasicBlock::Create(LLVM.context, funcAST.name + "-entry", func);
			LLVM.builder.SetInsertPoint(entry);

			// Stores the function for other functions to call it //

			LLVM.functions[funcAST.name] = func;

			// Creates the storer of the variables/parameters //

			FunctionScope funcScope(funcAST.params, func);

			// Generates the IR within the function by looping over the nodes //
			for (auto& node : funcAST.body)
			{
				ThrowIf<IRGenerationError>(IsValidTopLevelNode(node->m_Type) == false); // <- TODO: replace with actual error type

				Log::out<Log::Priority::HIGH>("Generating: ", node->TypeName());

				node->GenIR(LLVM, funcScope);
			}

			// Adds a terminator if there is none //
			if (entry->getTerminator() == nullptr)
			{
				LLVM.builder.CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(LLVM.context), 0, true));
			}

			// Verifies the function works //
			ThrowIf<IRGenerationError>(llvm::verifyFunction(*func, &llvm::errs())); // <- TODO: Make error type
		}

		catch (...)
		{
			__debugbreak();
			throw;
		}
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
			GenerateFunctionIR(func, LLVM);
		}

		// Outputs the IR to the output file //
		LLVM.module.print(file, nullptr);
	}
}
