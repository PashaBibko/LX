#pragma once

#include <Common/Macro.h>
#include <Common/Poly.h>

#include <Lexer/inc/Token.h>

#include <type_traits>
#include <vector>

namespace LX::FuncAST
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

	// Foward declaration of the node type
	class Node;

	// Base type for Nodes to inherit from
	class Node_T
	{
		private:
			// Friends the wrapper class for access
			friend class Node;

			// The type of the node
			NodeType m_Type;

			// The next node of the list
			Node_T* m_Next;

			// Returns the type of the Node (set by the constructor)
			inline NodeType Type() const
			{
				return m_Type;
			}

		protected:
			// Constructor to inherit from for setting type
			Node_T(NodeType type)
				: m_Type(type), m_Next(nullptr)
			{}

			// Virtual deconstructor because of polymorphism
			virtual ~Node_T()
			{
				// Delets the next in the list if it has been allocated
				if (m_Next != nullptr)
				{
					delete m_Next;
				}
			}
	};

	// Safe way to access the nodes
	class Node
	{
		private:
			Node_T* m_Ptr;
			Node_T* m_Root;

		public:
			// Constructor for the node
			Node(Node_T* ptr = nullptr)
				: m_Ptr(ptr), m_Root(nullptr)
			{}

			Node(Node_T& ref)
				: m_Ptr(&ref), m_Root(nullptr)
			{}

			// Returns true if not empty
			explicit operator bool() const { return m_Ptr != nullptr; }

			// Getter that returns nullptr if wrong type
			template<typename T> inline T* as() const
			{
				// Compile time check for correct type
				static_assert(std::is_base_of_v<Node_T, T>, "T must derive from Node_T");

				// Only returns the pointer if it is of that type
				return (T::EnumVal() == m_Ptr->Type()) ? (T*)m_Ptr : nullptr;
			}

			// Specialistion for the Node_T type as it should always be allowed
			template<> inline Node_T* as<Node_T>() const
			{
				return m_Ptr;
			}

			// Goes foward in the linked list
			void Next()
			{
				// Stops read access violations
				if (m_Ptr == nullptr)
				{
					throw std::runtime_error("Contents of Node are null");
				}

				// Else traverses the linked list
				m_Ptr = m_Ptr->m_Next;
			}

			// Pushes to the list
			template<typename T, typename... Args>
			void Push(Args&&... args)
			{
				// Compile time check for correct type
				static_assert(std::is_base_of_v<Node_T, T>, "T must derive from Node_T");

				// Checks if it is the first in the list in which case:
				// - We need to set it as the root
				// - Asign it to m_Ptr instead of m_Next
				if (m_Ptr == nullptr) _UNLIKELY
				{
					// Creates the object which is the root of the list
					m_Ptr = new T(std::forward<Args>(args)...);

					// So we store a pointer to it for later
					m_Root = m_Ptr;
				}

				// Else creates it as the next node
				else
				{
					// Creates the object as the next
					m_Ptr->m_Next = new T(std::forward<Args>(args)...);
					
					// And traverses foward
					m_Ptr = m_Ptr->m_Next;
				}
			}

			// Gets the current type
			NodeType CurrentType() const
			{
				// Stops read access violations
				if (m_Ptr == nullptr) { return NodeType::UNDEFINED; }
				
				// Else returns the type of the current node
				return m_Ptr->m_Type;
			}

			// Gets the root as a node type
			Node AsList() { return Node(m_Root); }
	};

	// -- All AST Nodes -- //
	// Constructors will be moved to a .cpp file "later"

	class Identifier : public Node_T
	{
		public:
			// Constructor
			Identifier(const std::string_view& name) :
				m_Name(name), Node_T(NodeType::IDENTIFIER)
			{}

			// Getter for polymorphism
			static constexpr NodeType EnumVal()
			{
				return NodeType::IDENTIFIER;
			}

			// Name of the identifier
			const std::string_view m_Name;

			// Will eventually hold information like namespace (parent)
	};

	class Declaration : public Node_T
	{
		public:
			// Constructor
			Declaration(Identifier name , Identifier type) :
				m_Name(name), m_Type(type), Node_T(NodeType::VAR_DEC)
			{}

			// Getter for polymorphism
			static constexpr NodeType EnumVal()
			{
				return NodeType::VAR_DEC;
			}

			// Type of the variable
			Identifier m_Type; // Will eventually need to hold more info such as ref or ptr

			// Name of the variable
			Identifier m_Name;
	};

	class Assignment : public Node_T
	{
		public:
			// Constructor
			Assignment(Identifier asignee, Node value) :
				m_Asignee(asignee), m_Value(value), Node_T(NodeType::VAR_ASSIGN)
			{}

			// Getter for polymorphism
			static constexpr NodeType EnumVal()
			{
				return NodeType::VAR_ASSIGN;
			}

			// The variable that is being assigned to
			Identifier m_Asignee;

			// Value that it is being set to
			Node m_Value;
	};

	class Operation : public Node_T
	{
		public:
			// Constructor
			Operation(Node lhs, TokenTypes::Func operand, Node rhs) :
				m_Lhs(lhs), m_Operand(operand), m_Rhs(rhs), Node_T(NodeType::OPERATION)
			{}

			// Getter for polymorphism
			static constexpr NodeType EnumVal()
			{
				return NodeType::OPERATION;
			}

			// Sides of the operation
			Node m_Lhs, m_Rhs;

			// Operation to apply to the sides
			TokenTypes::Func m_Operand;
	};

	class UnaryOperation : public Node_T
	{
		public:
			// Fancy boolean for which side the operation is
			enum class Side : bool
			{
				LEFT = false,
				RIGHT = true
			};

			// Constructor
			UnaryOperation(Node value, TokenTypes::Func operand, Side side = Side::LEFT)
				: m_Value(value), m_Operand(operand), m_Side(side), Node_T(NodeType::UNARY_OPERATION)
			{}

			// Getter for polymorphism
			static constexpr NodeType EnumVal()
			{
				return NodeType::UNARY_OPERATION;
			}

			// Operand
			TokenTypes::Func m_Operand;

			// Which side the operand is on
			Side m_Side;

			// What to apply the operand to
			Node m_Value;
	};

	class BracketExpression : public Node_T
	{
		public:
			// Constructor
			BracketExpression(Node expr) :
				m_Expression(expr), Node_T(NodeType::BRACKET_EXPR)
			{}

			// Getter for polymorphism
			static constexpr NodeType EnumVal()
			{
				return NodeType::BRACKET_EXPR;
			}

			// Expression within the brackets
			Node m_Expression;
	};

	class FunctionCall : public Node_T
	{
		public:
			// Constructor
			FunctionCall(Identifier name, std::vector<Node> arguments = std::vector<Node>())
				: m_Name(name), m_Arguments(arguments), Node_T(NodeType::FUNC_CALL)
			{}

			// Getter for polymorphism
			static constexpr NodeType EnumVal()
			{
				return NodeType::FUNC_CALL;
			}

			// Name of the function
			Identifier m_Name;

			// Arguments
			std::vector<Node> m_Arguments;
	};

	class StringLiteral : public Node_T
	{
		public:
			// Constructor
			StringLiteral(const std::string& value) :
				m_Value(value), Node_T(NodeType::STRING_LITERAL)
			{}

			// Getter for polymorphism
			static constexpr NodeType EnumVal()
			{
				return NodeType::STRING_LITERAL;
			}

			// Contents of the string literal
			const std::string m_Value;
	};

	class NumberLiteral : public Node_T
	{
		public:
			// Constructor
			NumberLiteral(const std::string& value) :
				m_Value(value), Node_T(NodeType::NUMBER_LITERAL)
			{}

			// Getter for polymorphism
			static constexpr NodeType EnumVal()
			{
				return NodeType::NUMBER_LITERAL;
			}

			// The number (yes it is stored as a string (yes its cursed))
			const std::string m_Value;
	};

	class IfStatement : public Node_T
	{
		public:
			// Constructor
			IfStatement(TokenTypes::Func type, Node condition) :
				m_Type(type), m_Condition(condition), Node_T(NodeType::IF_STATEMENT)
			{}

			// Getter for polymorphism
			static constexpr NodeType EnumVal()
			{
				return NodeType::IF_STATEMENT;
			}

			// Stores if it is { IF, ELIF, ELSE }
			TokenTypes::Func m_Type;

			// Condition. If m_Type == ELSE this is null
			Node m_Condition;

			// Body
			Node m_Body; // Linked list of the contents
	};

	class WhileLoop : public Node_T
	{
		public:
			// Constructor
			WhileLoop(Node condition) :
				m_Condition(condition), Node_T(NodeType::WHILE_LOOP)
			{}

			// Getter for polymorphism
			static constexpr NodeType EnumVal()
			{
				return NodeType::WHILE_LOOP;
			}

			// Condition of the loop
			Node m_Condition;

			// Body represented as a linked list
			Node m_Body;
	};

	class ForLoop : public Node_T
	{
		public:
			// Constructor
			ForLoop(Node condition) :
				m_Condition(condition), Node_T(NodeType::FOR_LOOP)
			{}

			// Getter for polymorphism
			static constexpr NodeType EnumVal()
			{
				return NodeType::FOR_LOOP;
			}

			// Condition of the loop
			Node m_Condition;

			// Body represented as a linked list
			Node m_Body;
	};

	class ReturnStatement : public Node_T
	{
		public:
			// Constructor
			ReturnStatement(Node value) :
				m_Value(value), Node_T(NodeType::RETURN_STATEMENT)
			{}

			// Getter for polymorphism
			static constexpr NodeType EnumVal()
			{
				return NodeType::RETURN_STATEMENT;
			}

			// Value that it can return
			Node m_Value;
	};

	class GoToStatement : public Node_T
	{
		public:
			// Constructor
			GoToStatement(TokenTypes::Func type) :
				m_GoToType(type), Node_T(NodeType::GOTO_STATEMENT)
			{}

			// Getter for polymorphism
			static constexpr NodeType EnumVal()
			{
				return NodeType::GOTO_STATEMENT;
			}
			
			// What type of go-to is it
			TokenTypes::Func m_GoToType;
	};
}

namespace LX
{
	struct FunctionAST
	{
		// The function declaration
		const std::vector<DecToken>& declaration;

		// Body of the function
		FuncAST::Node body;

		// Constructor
		FunctionAST(const std::vector<DecToken>& dec)
			: declaration(dec)
		{}
	};
}
