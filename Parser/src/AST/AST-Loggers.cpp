#include <LX-Common.h>

#include <AST.h>

namespace LX::AST
{
	void MultiNode::Log(unsigned depth)
	{
		throw int(); // <- TODO: Make an error for this
	}

	const char* MultiNode::TypeName()
	{
		throw int(); // <- TODO: Make an error for this
	}

	void NumberLiteral::Log(unsigned depth)
	{
		Log::out<Log::Priority::HIGH>(std::string(depth, '\t'), "Number: ", m_Number);
	}

	const char* NumberLiteral::TypeName()
	{
		return "Number Literal";
	}

	void Operation::Log(unsigned depth)
	{
		Log::out<Log::Priority::HIGH>(std::string(depth, '\t'), "Operation {", ToString(m_Operand), "}:");

		Log::out<Log::Priority::HIGH>(std::string(depth + 1, '\t'), "LHS:");
		m_Lhs->Log(depth + 2);

		Log::out<Log::Priority::HIGH>(std::string(depth + 1, '\t'), "RHS:");
		m_Rhs->Log(depth + 2);
	}

	const char* Operation::TypeName()
	{
		return "Operation";
	}

	void ReturnStatement::Log(unsigned depth)
	{
		Log::out<Log::Priority::HIGH, Log::Format::NONE>(std::string(depth, '\t'), "Return");

		if (m_Val != nullptr)
		{
			Log::out(':');
			m_Val->Log(depth + 1);
		}

		else
		{
			Log::out<Log::Priority::HIGH, Log::Format::NONE>('\n');
		}
	}

	const char* ReturnStatement::TypeName()
	{
		return "Return";
	}

	void VariableDeclaration::Log(unsigned depth)
	{
		Log::out<Log::Priority::HIGH>(std::string(depth, '\t'), "Variable declaration: ", m_Name);
	}

	const char* VariableDeclaration::TypeName()
	{
		return "Variable declaration";
	}

	void VariableAssignment::Log(unsigned depth)
	{
		Log::out<Log::Priority::HIGH>(std::string(depth, '\t'), "Variable assignment:");

		Log::out<Log::Priority::HIGH>(std::string(depth + 1, '\t'), "To: ", m_Name);
		Log::out<Log::Priority::HIGH>(std::string(depth + 1, '\t'), "Value:");
		m_Value->Log(depth + 2);
	}

	const char* VariableAssignment::TypeName()
	{
		return "Variable assignment";
	}

	void VariableAccess::Log(unsigned depth)
	{
		Log::out<Log::Priority::HIGH>(std::string(depth, '\t'), "Variable: ", m_Name);
	}

	const char* VariableAccess::TypeName()
	{
		return "Variable access";
	}

	void FunctionCall::Log(unsigned depth)
	{
		Log::out<Log::Priority::HIGH>(std::string(depth, '\t'), "Function call{", m_Name, "}:");

		if (m_Args.size() != 0)
		{
			Log::out<Log::Priority::HIGH>(std::string(depth + 1, '\t'), "Args:");

			for (auto& arg : m_Args) { arg->Log(depth + 2); }
		}
	}

	const char* FunctionCall::TypeName()
	{
		return "Function call";
	}
}
