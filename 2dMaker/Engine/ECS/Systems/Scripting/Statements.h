#pragma once
#include "Environment.h"

namespace D2Maker
{
	enum Operands
	{
		PLUS,
		SUBTRACT,
		MULTIPLY,
		DIVIDE
	};

	enum RelationalOperators   
	{
		EQUALITY,
		INEQUALITY,
		GREATER_THAN,
		LESS_THAN,
		GREATER_THAN_OR_EQUAL,
		LESS_THAN_OR_EQUAL,
		NUL

	};

	enum LogicOperators
	{
		AND,
		OR,
		NOT,
		NUL
	};

	class Statement
	{
	public:
		uint32_t line = 0;
		Statement(uint32_t line) : line(line) {}
		virtual void Execute() {};
	};


	class Expression
	{
	public:
		virtual ~Expression() = default;
	};

	class NumericExpression : public Expression
	{
	public:
		virtual Number evaluate() { return 0; }
		static float NumberToFloat(Number n) {
			return std::visit([](auto&& v)->float {return static_cast<float>(v);}, n);
		}
	};

	template<typename T>
	class ConstantExpression : public Expression
	{
		T value;
		ConstantExpression(T value) : value(value) {}
		T evaluate() { return value; }

	};
	template<typename T>
	class VariableExpression : public Expression
	{
		bool componentVariable false;
		T value;
		VariableExpression(const std::string& var)
		{
			value = Environment::Retrieve<T>(var);
		}
	};


	class ConstantNumber : public NumericExpression
	{
	private:
		Number value;
	public:
		ConstantNumber(Number value) : value(value) {}
		Number evaluate() { return value; }

	};
	class VariableNumber : public NumericExpression
	{
	private:
		Number value;
	public:
		VariableNumber(const std::string& var)
		{
			value = Environment::RetrieveNumber(var);
		}
		Number evaluate() { return value; }
	};


	class BinaryExpression : public NumericExpression
	{
	private:
		int Apply(int a, int b)
		{
			switch (operand)
			{
			case Operands::PLUS:
				return a + b;
			case Operands::SUBTRACT:
				return a - b;
			case Operands::MULTIPLY:
				return a * b;
			case Operands::DIVIDE:
				return a / b;
			}
		}

		float Apply(float a, float b)
		{
			switch (operand)
			{
			case Operands::PLUS:
				return a + b;
			case Operands::SUBTRACT:
				return a - b;
			case Operands::MULTIPLY:
				return a * b;
			case Operands::DIVIDE:
				return a / b;
			}
		}

		float Perform(int a, int b) { return static_cast<float>(Apply(a, b)); }
		float Perform(int a, float b) { return Apply(static_cast<float>(a), b); }
		float Perform(float a, int b) { return Apply(a, static_cast<float>(b)); }
		float Perform(float a, float b) { return Apply(a, b); }

	public:
		Operands operand = Operands::PLUS;
		std::unique_ptr<NumericExpression>& left;
		std::unique_ptr<NumericExpression>& right;
		BinaryExpression(std::unique_ptr<NumericExpression>& left, std::unique_ptr<NumericExpression>& right, char operandc) :
			left(left), right(right)
		{
			switch (operandc)
			{
			case '-':
				operand = Operands::SUBTRACT;
				break;
			case '*':
				operand = Operands::MULTIPLY;
				break;
			case '/':
				operand = Operands::DIVIDE;
				break;
			}



		}

		

		Number evaluate()
		{
			return Perform(NumberToFloat(left->evaluate()), NumberToFloat(right->evaluate()));
		}

	};


	class Condition : public Expression
	{
	public:
		virtual bool evaluate() = 0;

	};

	class BooleanExpression : public Condition{};
	
	template<typename T>
	class RelationalExpression : public BooleanExpression
	{
		T left;
		T right;
		RelationalOperators op=NUL;
		RelationalExpression(T left, T right, std::string opstr) : left(left),right(right)
		{
			char c = opstr[0]; //size controlled before creating the obj
			if (opstr.length() == 1)
			{
				switch (c)
				{
				case '>':
					op = GREATER_THAN;
					break;
				case '<':
					op = LESS_THAN;
					break;
				}
			}
			else if(opstr.length()==2)
			{
				switch (c)
				{
				case '=':
					op = EQUALITY;
					break;
				case '!':
					op = INEQUALITY;
					break;
				case '>':
					op = GREATER_THAN_OR_EQUAL;
					break;
				case '<':
					op = LESS_THAN_OR_EQUAL;
					break;
				}
			}
		}


		bool evaluate()
		{
			if constexpr (!std::is_same_v<T, Number> && !std::is_same_v<T, float> &&
				!std::is_same_v<T, int>)
			{
				switch (op)
				{
				case EQUALITY:
					return left == right;
				case INEQUALITY:
					return left != right;
				default:
					return false; //fallback
				}
			}
			else
			{
				float a = 0, b = 0;
				if constexpr (std::is_same_v<T, Number>)
				{
					a = NumericExpression::NumberToFloat(left);
					b = NumericExpression::NumberToFloat(right);
				}
				else
				{
					a = left;
					b = right;
				}

				switch (op)
				{
				case EQUALITY:
					return a == b;
				case INEQUALITY:
					return a != b;
				case GREATER_THAN:
					return a > b;
				case LESS_THAN:
					return a < b;
				case GREATER_THAN_OR_EQUAL:
					return a >= b;
				case LESS_THAN_OR_EQUAL:
					return a <= b;
				default:
					return false; // fallback
				}


			}
		}
	};

	class LogicExpression : public BooleanExpression
	{
		std::unique_ptr<BooleanExpression> &left;
		std::unique_ptr<BooleanExpression> &right;
		bool both = false;
		LogicOperators op = NUL;
		LogicExpression(std::unique_ptr<BooleanExpression>& leftin, std::unique_ptr<BooleanExpression>& rightin, std::string opstr)
			: left(leftin),right(rightin)
		{
			char c = opstr[0];
			switch (c)
			{
			case 'A':
				op = AND;
				break;
			case 'O':
				op = OR;
				break;
			case 'N':
				op = NOT;
				break;
			}
		}


		bool evaluate()
		{
			switch (op)
			{
			case AND:
				return left->evaluate() && right->evaluate();
			case OR:
				return left->evaluate() || right->evaluate();
			case NOT:
				if (left != nullptr) { return !left->evaluate(); }
			}

			return false; //fallback
		}
	};

	class KeyPressCondition : public Condition
	{
		Keys key = Keys::NUL;
		KeyPressCondition(std::string keystr)
		{
			key = Tokens::InterpretKey(keystr);
		}

		bool evaluate()
		{
			return EventManager::IsKeyPressed(key);
		}
	};

	class KeyClickCondition : public Condition
	{
		Keys key = Keys::NUL;
		KeyClickCondition(std::string keystr)
		{
			key = Tokens::InterpretKey(keystr);
		}
		bool evaluate()
		{
			return EventManager::IsKeyPressedOnce(key);
		}
	};

	class MouseClickCondition : public Condition
	{
		Entity entity = 0;
		Keys key = Keys::NUL;
		EntityManager& em;
		MouseClickCondition(Entity entity, Keys key,EntityManager& em) : em(em)
		{
			if (!(key == Keys::LEFT || key == Keys::RIGHT))
			{
				return;
			}
			
			this->key = key;
			this->entity = entity;

		}

		bool evaluate()
		{
			if (!em.hasComponent<Transform>(entity) || key==Keys::NUL)
			{
				return false;
			}
			if (EventManager::IsKeyPressedOnce(key))
			{
				Transform* t = em.getComponent<Transform>(entity);
				return ColliderFunctions::MousePointerCollisionWithRec({ t->x,t->y,t->width,t->height });
			}
			return false;

		}

		

	};






}



