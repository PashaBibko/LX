#pragma once

#include <Common/Poly.h>

#include <type_traits>
#include <memory>

namespace LX
{
	// Type to track the type of node to stop memory access violations
	enum class NodeType : short
	{
		IDENTIFIER,

		VAR_DEC,
		VAR_ASSIGN,

		OPERATION,
		UNARY_OPERATION,

		BRACKET_EXPR,

		FUNC_CALL,

		STRING_LITERAL,
		NUMBER_LITERAL,

		IF_STATEMENT,
		WHILE_LOOP,
		FOR_LOOP,

		RETURN_STATEMENT,
		GOTO_STATEMENT,

		UNDEFINED = -1
	};

	// Base type for Nodes to inherit from
	class Node_T : public PolyBase<NodeType, NodeType::UNDEFINED>
	{
		public:
			// Constructor to inherit from for setting type
			Node_T(NodeType type) : PolyBase(type) {}

			// Virtual because of polymorphism
			virtual ~Node_T() {}
	};

	class Identifier : public Node_T
	{
		public:
			Identifier() : Node_T(NodeType::IDENTIFIER) {}

			static constexpr NodeType EnumVal()
			{
				return NodeType::IDENTIFIER;
			}
	};

	class Declaration : public Node_T
	{
		public:
			Declaration() : Node_T(NodeType::VAR_DEC) {}

			static constexpr NodeType EnumVal()
			{
				return NodeType::VAR_DEC;
			}
	};

	class Assignment : public Node_T
	{
		public:
			Assignment() : Node_T(NodeType::VAR_ASSIGN) {}

			static constexpr NodeType EnumVal()
			{
				return NodeType::VAR_ASSIGN;
			}
	};

	class Operation : public Node_T
	{
		public:
			Operation() : Node_T(NodeType::OPERATION) {}

			static constexpr NodeType EnumVal()
			{
				return NodeType::OPERATION;
			}
	};

	class UnaryOperation : public Node_T
	{
		public:
			UnaryOperation() : Node_T(NodeType::UNARY_OPERATION) {}

			static constexpr NodeType EnumVal()
			{
				return NodeType::UNARY_OPERATION;
			}
	};

	class BracketExpression : public Node_T
	{
		public:
			BracketExpression() : Node_T(NodeType::BRACKET_EXPR) {}

			static constexpr NodeType EnumVal()
			{
				return NodeType::BRACKET_EXPR;
			}
	};

	class FunctionCall : public Node_T
	{
		public:
			FunctionCall() : Node_T(NodeType::FUNC_CALL) {}

			static constexpr NodeType EnumVal()
			{
				return NodeType::FUNC_CALL;
			}
	};

	class StringLiteral : public Node_T
	{
		public:
			StringLiteral() : Node_T(NodeType::STRING_LITERAL) {}

			static constexpr NodeType EnumVal()
			{
				return NodeType::STRING_LITERAL;
			}
	};

	class NumberLiteral : public Node_T
	{
		public:
			NumberLiteral() : Node_T(NodeType::NUMBER_LITERAL) {}

			static constexpr NodeType EnumVal()
			{
				return NodeType::NUMBER_LITERAL;
			}
	};

	class IfStatement : public Node_T
	{
		public:
			IfStatement() : Node_T(NodeType::IF_STATEMENT) {}

			static constexpr NodeType EnumVal()
			{
				return NodeType::IF_STATEMENT;
			}
	};

	class WhileLoop : public Node_T
	{
		public:
			WhileLoop() : Node_T(NodeType::WHILE_LOOP) {}

			static constexpr NodeType EnumVal()
			{
				return NodeType::WHILE_LOOP;
			}
	};

	class ForLoop : public Node_T
	{
		public:
			ForLoop() : Node_T(NodeType::FOR_LOOP) {}

			static constexpr NodeType EnumVal()
			{
				return NodeType::FOR_LOOP;
			}
	};

	class ReturnStatement : public Node_T
	{
		public:
			ReturnStatement() : Node_T(NodeType::RETURN_STATEMENT) {}

			static constexpr NodeType EnumVal()
			{
				return NodeType::RETURN_STATEMENT;
			}
	};

	class GoToStatement : public Node_T
	{
		public:
			GoToStatement() : Node_T(NodeType::GOTO_STATEMENT) {}

			static constexpr NodeType EnumVal()
			{
				return NodeType::GOTO_STATEMENT;
			}
	};

	class NodeAccseser
	{
		private:
			Node_T* ptr;

		public:
			explicit NodeAccseser(Node_T* _ptr)
				: ptr(_ptr) {}

			template<typename T>
			inline T* as()
			{
				static_assert(std::is_base_of_v<Node_T, T>, "T must derive from Node_T");

				return (T::EnumVal() == ptr->Type()) ? (T*)ptr : nullptr;
			}
	};
}
