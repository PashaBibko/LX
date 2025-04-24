#include <AST.h>

#include <fstream>

namespace LX::AST
{
	void Node::Log(std::ofstream* log, unsigned depth)
	{
		(*log) << std::string(depth, '\t') << "NULL node";
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
}
