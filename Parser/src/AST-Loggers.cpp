#include <AST.h>

namespace LX
{
	std::string ToString(std::unique_ptr<AST::Node>& node)
	{
		if (node == nullptr) { return "NULL Node"; }

		switch (node->m_Type)
		{
		case AST::Node::IDENTIFIER: return "IDENTIFIER";
		case AST::Node::OPERATION: return "OPERATION";
		case AST::Node::RETURN_STATEMENT: return "return";
		case AST::Node::NUMBER_LITERAL: return "number";
		}
	}
}
