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

	enum class RelationType
	{
		STRING,
		NUMERIC,
		INVALID
	};

	enum class PhysicsInstructionType {MOVE,SET};
	typedef PhysicsInstructionType PIT; // exclusively for shorting the ternary operator length

	
	class ASTBuilder
	{
	private:
		inline static ParseResult ParseStatement(const std::vector<std::vector<std::string>>& tokens, size_t& i, size_t&line,ScriptContext sc)
		{
			if (tokens[line].size() == 0) return {nullptr,false,"Empty Script"};
			else if (tokens[line][i] == "IF") { i++;return ParseIfStatement(tokens, i, line,sc); }
			else if (tokens[line][i] == "MOVE" || tokens[line][i]=="SET") 
			{ 
				i++;
				PhysicsInstructionType PIT_ = tokens[line][i] == "MOVE" ? PIT::MOVE : PIT::SET;
				return ParseSetMoveStatement(tokens, i, line, sc,PIT_); 
			}
			else return { nullptr,false,"Unknown Statement" };
		}

		inline static ParseResult ParseIfStatement(const std::vector<std::vector<std::string>>& tokens, size_t& i, size_t& line,ScriptContext sc)
		{
			ParseResult condition = ParseCondition(tokens[line], i);
			VALIDITY_CHECK(condition);
			if (!OutOfBounds(tokens[line], i, 0)) return { nullptr,false,"invalid if statement" };
			//since instructions should begin in a new line it checks if there is extra logic
			//this time we want to be OutOfBounds



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

		}

		inline static ParseResult ParseSetStatement(const std::vector<std::vector<std::string>>& tokens, size_t& i, size_t& line,ScriptContext sc)
		{
			
		}

		inline static ParseResult ParseCondition(const std::vector<std::string>& tokens, size_t& i)
		{
			if (OutOfBounds(tokens, i, 0)) return { nullptr,false,"invalid condition" };
			if (tokens[i] == "COLLIDE") { i++; return ParseCollideCondition(tokens, i); }
			if (tokens[i] == "KEYPRESS") { i++; return ParseKeypressCondition(tokens, i); }
			return ParseClassicCondition(tokens, i);
		}

		inline static ParseResult ParseCollideCondition(const std::vector<std::string>& tokens, size_t& i)
		{

		}

		inline static ParseResult ParseKeypressCondition(const std::vector<std::string>& tokens, size_t& i)
		{

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
			for (const auto& token : tokens) {
				if (token == "AND" || token == "OR" || token == "NOT") logicExpression = true;
				if (relationalOperators.find(token) != relationalOperators.end()) relationalExpression = true;
			}
			//logic expression has the priority
			if (logicExpression) {
				return ParseLogicExpression(tokens, i);
			}
			if (relationalExpression) {
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

		inline static bool AreSameType(ASTNode* a, ASTNode* b) {
			return (dynamic_cast<StringExpression*>(a) && dynamic_cast<StringExpression*>(b)) ||
				(dynamic_cast<NumericExpression*>(a) && dynamic_cast<NumericExpression*>(b));
		}
	};

	
}
