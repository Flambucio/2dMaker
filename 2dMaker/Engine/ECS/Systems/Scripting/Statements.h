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

	enum CoordType
	{
		X,
		Y,
		THETA,
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
	private:
	
		T left;
		T right;
		RelationalOperators op=NUL;
	public:
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
	private:
		std::unique_ptr<BooleanExpression> &left;
		std::unique_ptr<BooleanExpression> &right;
		bool both = false;
		LogicOperators op = LogicOperators::NUL;
	public:
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
	private:
		Keys key = Keys::NUL;
	public:
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
	private:
		Keys key = Keys::NUL;
	public:
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
	private:
		Entity entity = 0;
		Keys key = Keys::NUL;
		EntityManager& em;
	public:
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

	class CollideCondition : public Condition
	{
	private:
		Entity e1;
		Entity e2;
		EntityManager& em;
	public:
		CollideCondition(Entity e1, Entity e2,EntityManager&em) : e1(e1), e2(e2),em(em) {}
		bool evaluate()
		{
			if (!em.hasComponent<Collider>(e1) || !em.hasComponent<Collider>(e2))
			{
				return false;
			}

			Transform* t1 = em.getComponent<Transform>(e1);
			Transform* t2 = em.getComponent<Transform>(e2);
			return ColliderFunctions::CheckCollision({ t1->x,t1->y,t1->width,t1->height },
				{ t2->x,t2->y,t2->width,t2->height });

		}
	};

	class Instruction : public Statement
	{
	public:
		Instruction(uint32_t line) : Statement(line){}
	};

	class MoveStatement : public Instruction
	{
	private:
		EntityManager& em;
		Entity e = 0;
		CoordType c = CoordType::NUL;
		std::unique_ptr<NumericExpression> &expr;
		bool relative = false;
	public:
		MoveStatement(std::uint32_t line,EntityManager& em, Entity e, CoordType c, std::unique_ptr<NumericExpression>& expr,
			bool relative) : em(em),e(e),c(c),expr(expr),relative(relative) ,Instruction(line)
		{
		}

		void Execute()
		{
			if (!em.hasComponent<Velocity>(e)) return;
			Velocity* v = em.getComponent<Velocity>(e);
			float default_ = 0;
			float& ref=default_;
			switch (c)
			{
			case X:
				ref = v->dx;
				break;
			case Y:
				ref = (float)v->dy;
				break;
			case THETA:
				ref = (float)v->dtheta;
				break;

			}
			if (relative) ref += NumericExpression::NumberToFloat(expr->evaluate());
			else ref= NumericExpression::NumberToFloat(expr->evaluate());
		}

		


	};

	class SetStatement : public Instruction
	{
	private:
		EntityManager& em;
		Entity e = 0;
		CoordType c = CoordType::NUL;
		std::unique_ptr<NumericExpression>& expr;
		bool relative = false;
	public:
		SetStatement(std::uint32_t line, EntityManager& em, Entity e, CoordType c, std::unique_ptr<NumericExpression>& expr,
			bool relative) : em(em), e(e), c(c), expr(expr), relative(relative), Instruction(line)
		{
		}


		void Execute()
		{
			if (!em.hasComponent<Transform>(e)) return;
			Transform* t = em.getComponent<Transform>(e);
			float default_ = 0;
			float& ref = default_;
			switch (c)
			{
			case X:
				ref = t->x;
				break;
			case Y:
				ref = (float)t->y;
				break;
			case THETA:
				ref = (float)t->rotationDegrees;
				break;

			}
			if (relative) ref += NumericExpression::NumberToFloat(expr->evaluate());
			else ref = NumericExpression::NumberToFloat(expr->evaluate());
		}
	};



	class PlayInstruction : public Instruction
	{
	private:
		std::string audioName = "";
	public:
		PlayInstruction(std::string audioName, uint32_t line) : audioName(audioName), Instruction(line){}
		void execute()
		{
			if (AudioLoader::Exists(audioName))
			{
				AudioLoader::PlayAudio(audioName);
			}
		}

	};

	class IFStament : public Statement
	{
	private:
		std::unique_ptr<Condition> &condition;
		std::vector<std::unique_ptr<Instruction>>& instructions;
	public:
		IFStament(std::unique_ptr<Condition>& condition, 
			std::vector<std::unique_ptr<Instruction>>& instructions,uint32_t line) :
			condition(condition),instructions(instructions), Statement(line){ }
		void execute()
		{
			if (condition->evaluate())
			{
				for (const auto& i : instructions)
				{
					i->Execute();
				}
			}
		}


	};





	


}


	










