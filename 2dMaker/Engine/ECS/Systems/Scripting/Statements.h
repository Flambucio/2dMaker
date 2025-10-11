#pragma once
#include "Environment.h"

namespace D2Maker
{
	enum class Operands
	{
		PLUS,
		SUBTRACT,
		MULTIPLY,
		DIVIDE
	};

	enum class RelationalOperators   
	{
		EQUALITY,
		INEQUALITY,
		GREATER_THAN,
		LESS_THAN,
		GREATER_THAN_OR_EQUAL,
		LESS_THAN_OR_EQUAL,
		NUL

	};

	enum class LogicOperators
	{
		AND,
		OR,
		NOT,
		NUL
	};

	enum class CoordType
	{
		X,
		Y,
		THETA,
		NUL
	};

	class ASTNode
	{
	public:
		virtual ~ASTNode() = default;
	};

	class Statement : public ASTNode
	{
	public:
		uint32_t line = 0;
		Statement(uint32_t line) : line(line) {}
		virtual void Execute() {};
	};


	class Expression : public ASTNode
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

	class StringExpression : public Expression
	{
	public:
		virtual std::string evaluate() = 0;
	};

	class ConstantString : public StringExpression
	{
	private:
		std::string str = "";
	public:
		ConstantString(std::string str) : str(str) {}
		std::string evaluate() 
		{
			return str;
		}
	};

	class VariableString : public StringExpression
	{
	private:
		std::string variableName;
	public:
		VariableString(std::string variableName) : variableName(variableName) {}
		std::string evaluate()
		{
			return Environment::Retrieve<std::string>(variableName);
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
		std::string valueName;
	public:
		VariableNumber(const std::string& var) : valueName(var)
		{}
		Number evaluate() { return Environment::RetrieveNumber(valueName); }
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
		std::unique_ptr<NumericExpression> left;
		std::unique_ptr<NumericExpression> right;
		BinaryExpression(std::unique_ptr<NumericExpression> left, std::unique_ptr<NumericExpression> right, char operandc) :
			left(std::move(left)), right(std::move(right))
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

	class ConstantBool : public BooleanExpression
	{
	private:
		bool value;
	public:
		ConstantBool(bool value) : value(value) {}
		bool evaluate()
		{
			return value;
		}
	};

	class VariableBool : public BooleanExpression
	{
	private:
		std::string variableName;
	public:
		VariableBool(std::string variableName) : variableName(variableName) {}
		bool evaluate()
		{
			return Environment::Retrieve<bool>(variableName);
		}
	};
	
	template<typename T>
	class RelationalExpression : public BooleanExpression
	{
	private:
	
		std::unique_ptr<ASTNode> left;
		std::unique_ptr<ASTNode> right;
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
					op = RelationalOperators::GREATER_THAN;
					break;
				case '<':
					op = RelationalOperators::LESS_THAN;
					break;
				}
			}
			else if(opstr.length()==2)
			{
				switch (c)
				{
				case '=':
					op = RelationalOperators::EQUALITY;
					break;
				case '!':
					op = RelationalOperators::INEQUALITY;
					break;
				case '>':
					op = RelationalOperators::GREATER_THAN_OR_EQUAL;
					break;
				case '<':
					op = RelationalOperators::LESS_THAN_OR_EQUAL;
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
				case RelationalOperators::EQUALITY:
					return left == right;
				case RelationalOperators::INEQUALITY:
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
				case RelationalOperators::EQUALITY:
					return a == b;
				case RelationalOperators::INEQUALITY:
					return a != b;
				case RelationalOperators::GREATER_THAN:
					return a > b;
				case RelationalOperators::LESS_THAN:
					return a < b;
				case RelationalOperators::GREATER_THAN_OR_EQUAL:
					return a >= b;
				case RelationalOperators::LESS_THAN_OR_EQUAL:
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
		std::unique_ptr<BooleanExpression> left;
		std::unique_ptr<BooleanExpression> right;
		bool both = false;
		LogicOperators op = LogicOperators::NUL;
	public:
		LogicExpression(std::unique_ptr<BooleanExpression> leftin, std::unique_ptr<BooleanExpression> rightin, std::string opstr)
			: left(std::move(leftin)),right(std::move(rightin))
		{
			char c = opstr[0];
			switch (c)
			{
			case 'A':
				op = LogicOperators::AND;
				break;
			case 'O':
				op = LogicOperators::OR;
				break;
			case 'N':
				op = LogicOperators::NOT;
				break;
			}
		}


		bool evaluate()
		{
			switch (op)
			{
			case LogicOperators::AND:
				return left->evaluate() && right->evaluate();
			case LogicOperators::OR:
				return left->evaluate() || right->evaluate();
			case LogicOperators::NOT:
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
		std::unique_ptr<NumericExpression> expr;
		bool relative = false;
	public:
		MoveStatement(std::uint32_t line,EntityManager& em, Entity e, CoordType c, std::unique_ptr<NumericExpression> expr,
			bool relative) : em(em),e(e),c(c),expr(std::move(expr)),relative(relative) ,Instruction(line)
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
			case CoordType::X:
				ref = v->dx;
				break;
			case CoordType::Y:
				ref = (float)v->dy;
				break;
			case CoordType::THETA:
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
		std::unique_ptr<NumericExpression>expr;
		bool relative = false;
	public:
		SetStatement(std::uint32_t line, EntityManager& em, Entity e, CoordType c, std::unique_ptr<NumericExpression> expr,
			bool relative) : em(em), e(e), c(c), expr(std::move(expr)), relative(relative), Instruction(line)
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
			case CoordType::X:
				ref = t->x;
				break;
			case CoordType::Y:
				ref = (float)t->y;
				break;
			case CoordType::THETA:
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
		void Execute()
		{
			if (AudioLoader::Exists(audioName))
			{
				AudioLoader::PlayAudio(audioName);
			}
		}

	};

	class IfStament : public Statement
	{
	private:
		std::unique_ptr<Condition> condition;
		std::vector<std::unique_ptr<Instruction>> instructions;
	public:
		IfStament(std::unique_ptr<Condition> condition, 
			std::vector<std::unique_ptr<Instruction>> instructions,uint32_t line) :
			condition(std::move(condition)),instructions(std::move(instructions)), Statement(line){ }
		void Execute()
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