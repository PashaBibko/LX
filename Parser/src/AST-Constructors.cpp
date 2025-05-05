#include <LX-Common.h>

#include <Parser.h>

#include <AST.h>

namespace LX
{
	// Default constructor that just initalises LLVM variables that it holds //
	InfoLLVM::InfoLLVM(std::string name)
		: context{}, builder(context), module(name, context), scope(nullptr)
	{}

	// Reserves space for nodes (stops excess allocations) //
	FunctionDefinition::FunctionDefinition()
		: body{}, name{}
	{ body.reserve(32); }

	// Reserves space for functions (stops excess allocations) //
	FileAST::FileAST()
		: functions{}
	{ functions.reserve(8); }
}

namespace LX::AST
{
	// Passes constructor args to values //
	Node::Node(NodeType type)
		: m_Type(type)
	{}

	// Automatically sets type //
	MultiNode::MultiNode()
		: Node(Node::MULTI_NODE), nodes{}
	{}

	// Passes constructor args to values and sets type //
	NumberLiteral::NumberLiteral(std::string num)
		: Node(Node::NUMBER_LITERAL), m_Number(num)
	{}

	// Passes constructor args to values and sets type //
	Operation::Operation(std::unique_ptr<Node> lhs, Token::TokenType op, std::unique_ptr<Node> rhs)
		: Node(Node::OPERATION), m_Lhs(std::move(lhs)), m_Operand(op), m_Rhs(std::move(rhs))
	{}

	// Passes constructor args to values and sets type //
	ReturnStatement::ReturnStatement(std::unique_ptr<Node> val)
		: Node(Node::RETURN_STATEMENT), m_Val(std::move(val))
	{}

	// Passes constructor args to values and sets type //
	VariableDeclaration::VariableDeclaration(const std::string& name)
		: Node(Node::VARIABLE_DECLARATION), m_Name(name)
	{}

	// Passes constructor args to values and sets type //
	VariableAssignment::VariableAssignment(const std::string& name, std::unique_ptr<AST::Node> val)
		: Node(Node::VARIABLE_ASSIGNMENT), m_Name(name), m_Value(std::move(val))
	{}

	// Passes constructor args to values and sets type //
	VariableAccess::VariableAccess(const std::string& name)
		: Node(Node::VARIABLE_ACCESS), m_Name(name)
	{}
}
