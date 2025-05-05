#include <LX-Common.h>

#include <Parser.h>

#include <AST.h>

namespace LX
{
	llvm::AllocaInst* Scope::GetVar(const std::string& name)
	{
		// Stores a pointer to the current scope //
		Scope* current = this;

		do
		{
			// Checks if the variable exists in the current scope //
			bool exists = current->m_LocalVariables.contains(name);
			if (exists) { return m_LocalVariables[name]; }

			// Travels to the next scope //
			current = current->m_Child.get();

		} while (current != nullptr);

		// If it gets here it means it couldnt find the variable so it doesnt exist in the current context //
		return nullptr;
	}

	llvm::AllocaInst* Scope::CreateVar(const std::string& name, InfoLLVM& LLVM)
	{
		// Checks variable of the same name doesn't exist //
		ThrowIf<Scope::VariableAlreadyExists>(GetVar(name) != nullptr);

		// Else inserts it into the local set //
		m_LocalVariables[name] = LLVM.builder.CreateAlloca(LLVM.builder.getInt32Ty(), nullptr, name);
		return m_LocalVariables[name];
	}
}
