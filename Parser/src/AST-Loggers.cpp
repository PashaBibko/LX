#include <AST.h>

#include <LX-Common.h>

namespace LX::AST
{
	void MultiNode::Log(std::ofstream* log, unsigned depth)
	{
		throw int(); // <- TODO: Make an error for this
	}

	void NumberLiteral::Log(std::ofstream* log, unsigned depth)
	{
		(*log) << std::string(depth, '\t');
		(*log) << "Number: " << m_Number << "\n";
	}

	void Operation::Log(std::ofstream* log, unsigned depth)
	{
		(*log) << std::string(depth, '\t');
		(*log) << "Operation {" << ToString(m_Operand) << "}:\n";
		(*log) << std::string(depth + 1, '\t') << "LHS:\n";
		m_Lhs.get()->Log(log, depth + 2);
		(*log) << std::string(depth + 1, '\t') << "RHS:\n";
		m_Rhs.get()->Log(log, depth + 2);
	}

	void ReturnStatement::Log(std::ofstream* log, unsigned depth)
	{
		(*log) << std::string(depth, '\t');
		
		if (m_Val == nullptr)
		{
			(*log) << "Return\n";
		}
		
		else
		{
			(*log) << "Return:\n";
			m_Val->Log(log, depth + 1);
		}
	}

	void VariableDeclaration::Log(std::ofstream* log, unsigned depth)
	{
		(*log) << std::string(depth, '\t');
		(*log) << "Variable declaration: " << m_Name << "\n";
	}

	void VariableAssignment::Log(std::ofstream* log, unsigned depth)
	{
		(*log) << std::string(depth, '\t');
		(*log) << "Variable assignment:\n";
		(*log) << std::string(depth + 1, '\t') << "To: " << m_Name << "\n";
		(*log) << std::string(depth + 1, '\t') << "Value:\n";
		m_Value.get()->Log(log, depth + 2);
	}

	void VariableAccess::Log(std::ofstream* log, unsigned depth)
	{
		(*log) << std::string(depth, '\t');
		(*log) << "Variable: " << m_Name << '\n';
	}
}
