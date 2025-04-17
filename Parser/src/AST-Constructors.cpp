#include <Parser.h>

namespace LX::AST
{
	NumberLiteral::NumberLiteral(std::string num)
		: Node(Node::NUMBER_LITERAL), m_Number(num)
	{}

	Operation::Operation(std::unique_ptr<Node> lhs, Token::TokenType op, std::unique_ptr<Node> rhs)
		: Node(Node::OPERATION), m_Lhs(std::move(lhs)), m_Operand(op), m_Rhs(std::move(rhs))
	{}

	ReturnStatement::ReturnStatement(std::unique_ptr<Node> val)
		: Node(Node::RETURN_STATEMENT), m_Val(std::move(val))
	{}
}
