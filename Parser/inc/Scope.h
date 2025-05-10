#pragma once

#include <LX-Common.h>

#include <AST.h>

namespace LX
{
	CREATE_EMPTY_LX_ERROR_TYPE(VariableError);

	class FunctionScope
	{
		public:
			FunctionScope(const std::vector<std::string> paramNames, llvm::Function* func)
			{
				// Counter for the args //
				unsigned argCounter = 0;

				// Checks the parameter does not exist before inserting //
				for (const std::string& param : paramNames)
				{
					ThrowIf<VariableError>(GetVarLocation(param) != NONE);

					// Adds the argument to the map and sets its name //
					m_Params[param] = func->getArg(argCounter);
					m_Params[param]->setName(param);

					// Iterates to the next one //
					argCounter++;
				}
			}

			llvm::Value* DecVar(const std::string& name, InfoLLVM& LLVM)
			{
				// Finds out if the variable already exists //
				ThrowIf<VariableError>(GetVarLocation(name) != NONE);

				// Allocates the variable and then returns a pointer to it's allocation //
				llvm::AllocaInst* inst = LLVM.builder.CreateAlloca(LLVM.builder.getInt32Ty(), nullptr, name);
				m_LocalVars[name] = inst;
				return inst;
			}

			llvm::Value* AccessVar(const std::string& name, InfoLLVM& LLVM)
			{
				VariableLocation l = GetVarLocation(name);

				switch (l)
				{
					case LOCAL:
						return LLVM.builder.CreateLoad(LLVM.builder.getInt32Ty(), m_LocalVars[name], name + "_v");

					case PARAMS:
						return m_Params[name];

					default:
						throw VariableError();
				}
			}

			llvm::Value* AssignVar(const std::string& name, AST::Node* value, InfoLLVM& LLVM, FunctionScope& scope)
			{
				// Checks it is a local variable and not a parameter //
				ThrowIf<VariableError>(GetVarLocation(name) != LOCAL);
				
				// Returns a pointer to the assignment in the builder //
				return LLVM.builder.CreateStore(value->GenIR(LLVM, scope), m_LocalVars[name]);
			}

		protected:
			enum VariableLocation
			{
				NONE		= 0,
				PARAMS		= 1,
				LOCAL		= 2
			};

			VariableLocation GetVarLocation(const std::string& name)
			{
				// Searches in the variable maps //
				auto pIt = m_Params.find(name);
				auto lIt = m_LocalVars.find(name);

				if (pIt != m_Params.end())
				{
					return PARAMS;
				}

				if (lIt != m_LocalVars.end())
				{
					return LOCAL;
				}

				return NONE;
			}

		private:
			// Holds the parameters of the functions //
			std::unordered_map<std::string, llvm::Argument*> m_Params;

			// Holds all local variables //
			std::unordered_map<std::string, llvm::AllocaInst*> m_LocalVars;
	};
}
