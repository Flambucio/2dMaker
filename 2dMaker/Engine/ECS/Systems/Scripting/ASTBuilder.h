#pragma once
#include "Statements.h"
#define VALIDITY_CHECK(var)  if(!var.success) return var //solo in unary per il momento
#define OUTOFBOUNDS(uint) OutOfBounds(tokens,i,uint)
//TODO : sostituire se funziona
namespace D2Maker
{

	struct ParseResult
	{
		std::unique_ptr<ASTNode> node;
		bool success;
		std::string errorMessage;
	};

	struct CheckIfDepthResult
	{
		bool success;
		std::string message;

	};

	enum class RelationType
	{
		STRING,
		NUMERIC,
		INVALID
	};

	enum class KeyConditionType 
	{
		REGULAR,
		CLICK
	};

	enum class PhysicsInstructionType {MOVE,SET};
	typedef PhysicsInstructionType PIT; // exclusively for shorting length

	
	class ASTBuilder
	{
	public:
		inline static std::vector<std::unique_ptr<Statement>> ParseScript(std::vector<std::vector<std::string>> tokens,ScriptContext sc)
		{
			std::vector < std::unique_ptr<Statement>> statements;
			for (size_t line = 0;line < tokens.size();line++)
			{
				size_t i = 0;
				ParseResult statement_ = ParseStatement(tokens,i,line,sc);
				if (!statement_.success) 
				{ 
					CONSOLELOG(statement_.errorMessage); 
					return {};
				}
				std::unique_ptr<Statement> currentStatementCasted(dynamic_cast<Statement*>(statement_.node.release()));
				if (!currentStatementCasted)
				{
					CONSOLELOG("failed to interpret logic at line " + std::to_string(line));
					return {};
				}
				statements.push_back(std::move(currentStatementCasted));
			}
		}
	private:
		
		inline static CheckIfDepthResult CheckIfsDepth(const std::vector<std::vector<std::string>>& tokens,std::string& message)
		{
			unsigned int ifDepth = 0;
			for (int i=0; i < tokens.size();i++) 
			{
				if (tokens[i].size() == 0) continue;
				const std::string& word = tokens[i][0];

				if (word == "IF") ifDepth++;
				else if (word == "ENDIF")
				{
					if (ifDepth == 0) return{ false,"ENDIF closes nothing" };
					ifDepth--;
				}
			}

			// alla fine
			if (ifDepth != 0) 
			{
				return { false,"IF not closed" };
			}
			return { true,"success" };
		}


		inline static ParseResult ParseStatement(const std::vector<std::vector<std::string>>& tokens, size_t& i, size_t&line,ScriptContext sc)
		{
			if (tokens[line].size() == 0) return { nullptr,false,"Empty Script" };
			else if (tokens[line][i] == "IF") { i++;return ParseIfStatement(tokens, i, line, sc); }
			else if (tokens[line][i] == "MOVE" || tokens[line][i] == "SET")
			{
				i++;
				PhysicsInstructionType PIT_ = tokens[line][i] == "MOVE" ? PIT::MOVE : PIT::SET;
				return ParseSetMoveStatement(tokens, i, line, sc, PIT_);
			}
			else if (tokens[line][i] == "ASSIGN")  ParseAssigmentInstruction(tokens[i], i, line);
			else return { nullptr,false,"Unknown Statement" };
		}

		inline static ParseResult ParseIfStatement(const std::vector<std::vector<std::string>>& tokens, size_t& i, size_t& line,ScriptContext sc)
		{
			ParseResult condition = ParseCondition(tokens[line], i,sc);
			VALIDITY_CHECK(condition);
			std::unique_ptr<Condition> cond(dynamic_cast<Condition*>(condition.node.release()));
			if (!OutOfBounds(tokens[line], i, 0)) return { nullptr,false,"invalid if statement" };
			
			//since instructions should begin in a new line it checks if there is extra logic
			//this time we want to be OutOfBounds
			line++;
			i = 0;
			bool success = true;
			std::vector<std::unique_ptr<Statement>> statements;
			while (tokens[line][0] != "ENDIF")
			{
				ParseResult pr = ParseStatement(tokens, i, line, sc);
				VALIDITY_CHECK(pr);
				std::unique_ptr<Statement> statement(
					dynamic_cast<Statement*>(pr.node.release())
				);
				if (statement == nullptr) return { nullptr,false,"invalid instruction inside if statement" };
				statements.push_back(std::move(statement));

			}


			return { std::make_unique<IfStament>(std::move(cond),std::move(statements),line) };

		}

		
		inline static ParseResult ParseAssigmentInstruction(const std::vector<std::string>& tokens, size_t& i,size_t&line)
		{
			if (OutOfBounds(tokens, i, 0)) return { nullptr,false,"invalid assigment instruction" };
			std::string varStr = tokens[i];
			i++;
			ParseResult VTATV = ParseBinaryorValueExpression(tokens, i);
			//valueToAssignToVar
			VALIDITY_CHECK(VTATV);
			std::unique_ptr<Expression> expr(dynamic_cast<Expression*>(VTATV.node.release()));
			if (!expr) return { nullptr,false,"invalid expression in assigment instruction" };
			Type returnValueExpressionType = DeduceType(VTATV.node);
			if (returnValueExpressionType == Type::NUL) return { nullptr,false,"invalid expression type assigment instruction" };
			Type varType = Environment::RetrieveType(varStr);
			if (!(
				(
					(varType == Type::FLOAT || varType == Type::INT) && returnValueExpressionType == Type::NUMERIC
				) || varType == returnValueExpressionType)
			   )
			{
				return { nullptr,false,"types not matching in assigment instruction" };
			}

			return { std::make_unique<AssigmentInstruction>(varStr,std::move(expr),varType,line),true,"success"};
			


		}
		

		inline static ParseResult ParseSetMoveStatement(const std::vector<std::vector<std::string>>& tokens, size_t& i, size_t& line, ScriptContext sc,PhysicsInstructionType PIT_)
		{
			if (OutOfBounds(tokens[line], i, 0))
			{
				return { nullptr,false,"invalid move/set statement" };
			}
			CoordType ct = CoordType::NUL;
			if (tokens[line][i] == "X") ct = CoordType::X;
			else if (tokens[line][i] == "Y") ct = CoordType::Y;
			else if (tokens[line][i] == "THETA") ct = CoordType::THETA;
			else return { nullptr,false,"invalid coordinate in move/set statement" };
			ParseResult value = ParseBinaryorValueExpression(tokens[line], i);
			VALIDITY_CHECK(value);
			auto valuePtr = dynamic_cast<NumericExpression*>(value.node.get());
			if (!valuePtr) return { nullptr,false,"invalid value in move/set statement" };
			std::unique_ptr<NumericExpression> valueCasted (dynamic_cast<NumericExpression*>(value.node.release()));
			bool relative = false;
			if (!OutOfBounds(tokens[line], i, 0))
			{
				if (tokens[line][i] == "RELATIVE") relative = true;
				else return { nullptr,false,"invalid end in move/set statement" };
			}

			if (PIT_ == PIT::MOVE) return { std::make_unique<MoveStatement>(line + 1,sc.em,sc.e,ct,std::move(valueCasted),relative) };
			else return { std::make_unique<SetStatement>(line + 1,sc.em,sc.e,ct,std::move(valueCasted),relative) };

		}

		inline static ParseResult ParseCondition(const std::vector<std::string>& tokens, size_t& i,ScriptContext& sc)
		{
			if (OutOfBounds(tokens, i, 0)) return { nullptr,false,"invalid condition" };
			if (tokens[i] == "COLLIDE") { i++; return ParseCollideCondition(tokens, i,sc); }
			if (tokens[i] == "KEYPRESS" || tokens[i]=="KEYCLICK") { return ParseKeypressCondition(tokens, i); }
			if (tokens[i] == "TIMER") { i++; return ParseTimerCondition(tokens, i); }
			return ParseClassicCondition(tokens, i);
		}

		inline static ParseResult ParseCollideCondition(const std::vector<std::string>& tokens, size_t& i,ScriptContext& sc)
		{
			if (OutOfBounds(tokens, i, 1)) return { nullptr,false,"invalid collide condition" };
			std::array<Entity, 2> collideEntities;
			for (int j = 0;i < 2;j++) 
			{
				if (j==0 && tokens[i] == "this") collideEntities[j] = sc.e;
				else
				{
					if (!sc.em.isAlive(tokens[i])) return { nullptr,false,"entity does not exist" };
					collideEntities[j] = sc.em.entityNames[tokens[i]];
					i++;
				}
			}

			return { std::make_unique<CollideCondition>(collideEntities[0],collideEntities[1],sc.em),true,"success" };


		}

		inline static ParseResult ParseKeypressCondition(const std::vector<std::string>& tokens, size_t& i)
		{
			typedef KeyConditionType KCT;
			KeyConditionType kct = tokens[i] == "KEYPRESS" ? KCT::REGULAR : KCT::CLICK;
			i++;
			if (OutOfBounds(tokens, i, 0)) return { nullptr,false,"invalid keycondition" };
			std::string key = tokens[i];
			i++;
			if (kct == KCT::REGULAR) return { std::make_unique<KeyPressCondition>(key),true,"success" };
			else return { std::make_unique<KeyClickCondition>(key),true,"success" };
		}

		inline static ParseResult ParseTimerCondition(const std::vector<std::string>& tokens, size_t& i)
		{
			if (OutOfBounds(tokens, i, 0)) return { nullptr,false,"invalid timer condition" };
			double val = 0;
			if (!ConvertStringToNum<double>(tokens[i], val)) return { nullptr,false,"invalid timer condition value" };
			return { std::make_unique<TimerCondition>(val),true,"success" };
		}


		inline static ParseResult ParseClassicCondition(const std::vector<std::string>& tokens, size_t& i)
		{
			if (!OutOfBounds(tokens, i, 1) && Environment::Exists(tokens[i + 1], Type::BOOL))
			{
				return { std::make_unique<VariableBool>(tokens[i+1]),true,"success"};
				//dont need to increment cause if the condition is complete the statement is complete and
				//skips to next line
			}
			else if (tokens[i].size() < 3)
			{
				return { nullptr,false,"Invalid Condition" };
			}
			static const std::unordered_set<std::string> relationalOperators = { ">","<","==","!=",">=","<=" };
			bool logicExpression = false;
			bool relationalExpression = false;
			for (const auto& token : tokens) 
			{
				if (token == "AND" || token == "OR" || token == "NOT") logicExpression = true;
				if (relationalOperators.find(token) != relationalOperators.end()) relationalExpression = true;
			}
			//logic expression has the priority
			if (logicExpression) 
			{
				return ParseLogicExpression(tokens, i);
			}
			if (relationalExpression) 
			{
				return ParseRelationalExpression(tokens, i);
			}

			return { nullptr,false,"invalid boolean expression" };
		}

		inline static ParseResult ParseLogicExpression(const std::vector<std::string>& tokens, size_t& i)
		{
			if (tokens[i] == "NOT")
			{
				i++;
				return ParseLogicExpressionUnary(tokens, i);
			}
			
			return ParseLogicExpressionBinary(tokens, i);

		}

		inline static ParseResult ParseRelationalExpressionOrBool(const std::vector<std::string>&tokens, size_t&i)
		{
			if (OutOfBounds(tokens,i,1)) 
			{
				return { nullptr,false,"invalid expression" };
			}
			if (tokens[i] == "VAR" && Environment::Exists(tokens[i + 1], Type::BOOL))
			{
				ParseResult r= { std::make_unique<VariableBool>(tokens[i + 1]) ,true,"success" };
				i += 2;
				return r;
			}

			return ParseRelationalExpression(tokens, i);
		}

		inline static ParseResult ParseLogicExpressionBinary(const std::vector<std::string>& tokens, size_t& i)
		{
			ParseResult left=ParseRelationalExpressionOrBool(tokens,i);
			VALIDITY_CHECK(left);
			std::unique_ptr<BooleanExpression> leftBoolean(dynamic_cast<BooleanExpression*>(left.node.release()));
			if (!leftBoolean) return { nullptr,false,"invalid value in logic expression" };
			if (OutOfBounds(tokens, i, 0)) return { nullptr,false,"invalid logic expression" };
			if (tokens[i] != "AND" && tokens[i] != "OR")
			{
				return { nullptr,false,"invalid logic operator" };
			}
			std::string op = tokens[i];
			i++;
			ParseResult right = ParseRelationalExpressionOrBool(tokens, i);
			VALIDITY_CHECK(right);
			std::unique_ptr<BooleanExpression> rightBoolean(dynamic_cast<BooleanExpression*>(right.node.release()));
			if (!rightBoolean) return { nullptr,false,"invalid value in logic expression" };
			return { std::make_unique<LogicExpression>(std::move(leftBoolean),std::move(rightBoolean),op) };

			
		}

		inline static ParseResult ParseLogicExpressionUnary(const std::vector<std::string>& tokens, size_t& i)
		{
			//not expressions
			ParseResult rInside = ParseRelationalExpressionOrBool(tokens, i);
			VALIDITY_CHECK(rInside);
			std::unique_ptr<BooleanExpression> rInsideCasted(dynamic_cast<BooleanExpression*>(rInside.node.release()));
			if (!rInsideCasted) return { nullptr,false,"invalid value in logic expression" };
			ParseResult r = { std::make_unique <LogicExpression>(std::move(rInsideCasted),nullptr,"NOT") };
			return r;
		}

		

		

		inline static ParseResult ParseRelationalExpression(const std::vector<std::string>&tokens,size_t&i)
		{
			static const std::unordered_set<std::string> operators = { "==","!=",">","<",">=","<=" };
			ParseResult left = ParseBinaryorValueExpression(tokens, i);
			VALIDITY_CHECK(left);
			if (OutOfBounds(tokens, i, 0)) return { nullptr,false,"invalid relational expression" };
 			if (operators.find(tokens[i]) == operators.end()) return { nullptr,false,"invalid operator" };
			std::string c = tokens[i];
			i++;
			ParseResult right = ParseBinaryorValueExpression(tokens, i);
			VALIDITY_CHECK(right);
			return MakeRelationalExpression(left, right, c);
		}

		inline static ParseResult MakeRelationalExpression(ParseResult& left,ParseResult& right, std::string op)
		{
			RelationType rtype = RelationType::INVALID;
			if (!AreSameType(left.node.get(),right.node.get())) return { nullptr,false,"different types in relational expression" };
			return { std::make_unique<RelationalExpression>(std::move(left.node),std::move(right.node),op),true,"success" };
			
		}

		inline static ParseResult ParseBinaryorValueExpression(const std::vector<std::string>& tokens, size_t& i)
		{
			static std::unordered_set<std::string> operators = { "+","-","*","/" };
			int foundcount = 0;
			for (int j = i;j < i+5 && j < tokens.size();j++)
			{
				
				if (operators.find(tokens[j]) != operators.end())
				{
					foundcount++;
				}
			}

			if (foundcount > 1) return { nullptr,false,"invalid operator" };
			else if (foundcount == 1) return ParseBinaryExpression(tokens, i);
			else return ParseValueExpression(tokens, i);
		}

		inline static ParseResult ParseBinaryExpression(const std::vector<std::string>& tokens, size_t& i)
		{
			static std::unordered_set<std::string> operators = { "+","-","*","/" };
			ParseResult resultLeft = ParseValueExpression(tokens, i);
			VALIDITY_CHECK(resultLeft);
			auto resultLeftptr = dynamic_cast<NumericExpression*> (resultLeft.node.get());
			if (!resultLeftptr)
			{
				return { nullptr,false,"binary expressions must have numeric values on both sides" };
			}

			if (OutOfBounds(tokens, i, 0)) return { nullptr,false,"invalid binary expression" };
			if (operators.find(tokens[i]) == operators.end()) return { nullptr,false,"invalid binary expression operator" };

			//cant be outofbounds cause it needs at least 1 character to exist
			//in the first place
			char c = char(tokens[i][0]);
			i++;

			ParseResult resultRight = ParseValueExpression(tokens, i);
			VALIDITY_CHECK(resultRight);
			auto resultRightptr = dynamic_cast<NumericExpression*>(resultRight.node.get());
			if (!resultRightptr)
			{
				return { nullptr,false,"binary expressions must have numeric values on both sides" };
			}

			std::unique_ptr<NumericExpression> leftNumeric(
				dynamic_cast<NumericExpression*>(resultLeft.node.release())
			);
			std::unique_ptr<NumericExpression> rightNumeric(
				dynamic_cast<NumericExpression*>(resultRight.node.release())
			);

			return { std::make_unique<BinaryExpression>(std::move(leftNumeric),
				std::move(rightNumeric),c) };
			

			
		}

		inline static ParseResult ParseValueExpression(const std::vector<std::string>& tokens, size_t& i)
		{
			ParseResult result = { nullptr,false,"not executed" };
			if (tokens[i] == "VAR")
			{
				if (OutOfBounds(tokens, i, 1))
				{
					return { nullptr,false,"invalid value/variable" };
				}
				
				if (Environment::Exists(tokens[i + 1], Type::STRING))
				{
					result = { std::make_unique<VariableString>(tokens[i + 1]),true,"success" };
				}
				else if (Environment::Exists(tokens[i + 1], Type::INT) || Environment::Exists(tokens[i + 1], Type::FLOAT))
				{
					result = { std::make_unique<VariableNumber>(tokens[i + 1]),true,"success" };
				}
				else if (Environment::Exists(tokens[i + 1], Type::BOOL))
				{
					result = { std::make_unique<VariableBool>(tokens[i + 1]),true,"success" };
				}
				else
				{
					result = { nullptr,false,"variable not found" };
				}
				
				i += 2;

			}
			else
			{
				if (OutOfBounds(tokens, i, 1))
				{
					return { nullptr,false,"invalid value/variable" };
				}
				float f = 0;
				if (tokens[i] == "true" || tokens[i] == "false")
				{
					bool value = tokens[i] == "true" ? true : false;
					result= { std::make_unique<ConstantBool>(value),true,"success" };
				}
				else if (ConvertStringToNum<float>(tokens[i], f)) 
				{
					result= { std::make_unique<ConstantNumber>(f),true,"success" };
				}
				else
				{
					result = { std::make_unique<ConstantString>(tokens[i]),true,"success"};
				}

				i++;

				
			}

			return result;
		}

		inline static bool OutOfBounds(const std::vector<std::string> &v, const size_t &i, const uint32_t &toAccess)
		{
			return i + toAccess >= v.size();
		}

		inline static Type DeduceType(std::unique_ptr<ASTNode>& ptr)
		{
			enum SCT { NUMERIC_, BOOL_,STRING_}; //"_" used to differentiate this from the enum class in Environment
			std::array<bool, 3> successConvertingTo = {};
			successConvertingTo[NUMERIC_] = dynamic_cast<NumericExpression*>(ptr.get())!=nullptr;
			successConvertingTo[BOOL_] = dynamic_cast<BooleanExpression*>(ptr.get())!=nullptr;
			successConvertingTo[STRING_] = dynamic_cast<StringExpression*>(ptr.get())!=nullptr;
			if (successConvertingTo[NUMERIC_]) return Type::NUMERIC;
			else if (successConvertingTo[BOOL_]) return Type::BOOL;
			else if (successConvertingTo[STRING_]) return Type::STRING;
			return Type::NUL;
		}

		inline static bool AreSameType(ASTNode* a, ASTNode* b) {
			return (dynamic_cast<StringExpression*>(a) && dynamic_cast<StringExpression*>(b)) ||
				(dynamic_cast<NumericExpression*>(a) && dynamic_cast<NumericExpression*>(b));
		}


	};

	
}
