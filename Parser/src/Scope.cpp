#include <Parser.h>

#include <Util.h>

namespace LX
{
	bool Scope::DoesVarExist(const std::string& name)
	{
		// Stores a pointer to the current scope //
		Scope* current = this;

		do
		{
			// Checks if the variable exists in the current scope //
			bool exists = current->m_LocalVariables.contains(name);
			if (exists) { return true; }

			// Travels to the next scope //
			current = current->m_Child.get();

		} while (current != nullptr);

		// If it gets here it means it couldnt find the variable so it doesnt exist in the current context //
		return false;
	}

	void Scope::CreateVar(const std::string& name)
	{
		// Checks variable of the same name doesn't exist //
		ThrowIf<Scope::VariableAlreadyExists>(DoesVarExist(name));

		// Else inserts it into the local set //
		m_LocalVariables.insert(name);
	}
}
