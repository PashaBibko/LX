#include <Parser.h>

#include <Util.h>

namespace LX
{
	// Util function for getting a pointer to an item from a map //
	static inline Scope::Variable* GetFromMap(const std::string& name, std::unordered_map<std::string, Scope::Variable>& map)
	{
		// Checks if it is in a map and if so returns it //
		if (auto it = map.find(name); it != map.end()) { return &it->second; }

		// Else returns null //
		else { return nullptr; }
	}

	// Base logic for getting a variable by it's name without any error checking //
	Scope::Variable* Scope::GetVarOfNameImpl(const std::string& name)
	{
		// Stores the current scope that is being checked for the variable //
		Scope* current = this;

		// Loops over the scope and it's child to find the variable //
		do
		{
			// Gets the variable (if it exists) //
			Variable* var = GetFromMap(name, current->m_LocalVariables);

			// Returns the variable if it exists //
			if (var != nullptr) { return var; }

			// Assigns current to the child to recursively check //
			// Doing it like this avoids recursive functions //
			current = current->m_Child.get();

		} while (current != nullptr);

		// Else returns a nullptr and lets the caller handle the error checking //
		return nullptr;
	}

	// Gets a variable from the scope by it's name //
	Scope::Variable* Scope::GetVarOfName(const std::string& name)
	{
		// Gets the variable (if it exists) //
		Variable* var = GetVarOfNameImpl(name);

		// Throws an error if the variable doesn't exist //
		ThrowIf<VariableDoesntExist>(var == nullptr);

		// Else it can return the variable //
		return var;
	}

	// Creates a variable of the given name, returns nullptr if a variable with that name already exists //
	Scope::Variable* Scope::CreateVar(const std::string& name)
	{
		// Checks if a variable with the same name already exists //
		Variable* alreadyExist = GetVarOfNameImpl(name);

		// Throws an error if the variable already exists //
		ThrowIf<VariableAlreadyExists>(alreadyExist != nullptr);

		// Else creates the variable and returns it //
		return &m_LocalVariables[name];
	}
}
