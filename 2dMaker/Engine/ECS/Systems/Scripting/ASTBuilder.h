#pragma once
#include "Statements.h"
namespace D2Maker
{
	struct ParseResult
	{
		std::unique_ptr<ASTNode> node;
		bool success;
		std::string errorMessage;
	};


	class ASTBuilder
	{
	private:
		inline static ParseResult ParseStatement(const std::vector<std::vector<std::string>>& tokens, size_t& i, size_t&line)
		{
			if (tokens[line].size() == 0) return {nullptr,false,"Empty Script"};
			else if (tokens[line][i] == "IF") return ParseIfStatement(tokens, i, line);
			else if (tokens[line][i] == "MOVE") return ParseMoveStatement(tokens, i,line);
			else if (tokens[line][i] == "SET") return ParseSetStatement(tokens, i,line);
			else return { nullptr,false,"Unknown Statement" };
		}

		inline static ParseResult ParseIfStatement(const std::vector<std::vector<std::string>>& tokens, size_t& i, size_t& line)
		{
			i++;
			

		}

		inline static ParseResult ParseMoveStatement(const std::vector<std::vector<std::string>>& tokens, size_t& i, size_t& line)
		{

		}

		inline static ParseResult ParseSetStatement(const std::vector<std::vector<std::string>>& tokens, size_t& i, size_t& line)
		{

		}

		inline static ParseResult ParseCondition(const std::vector<std::string>& tokens, size_t& i)
		{
			if (tokens[i].size() == 2 && Environment::Exists(tokens[i + 1], Type::BOOL))
			{
				
				return { std::make_unique<VariableBool>(tokens[i+1]),true,"success"};
			}
			else if (tokens[i].size() < 3)
			{
				return { nullptr,false,"Invalid Condition" };
			}
			bool logicExpression = false;
			for (const auto& token : tokens) {
				if (token == "AND" || token == "OR" || token == "NOT") logicExpression = true;
			}

			if (logicExpression) {
				return ParseLogicExpression(tokens, i);
			}
			
			/*
			else if (tokens[i] == "NOT")
			{
				if (tokens[i + 1] == "VAR" && Environment::Exists(tokens[i+2],Type::BOOL))
				{
					return { std::make_unique <VariableBool>(tokens[i + 2]),true,"success" };
				}
			}*/
		}

		inline static ParseResult ParseLogicExpression(const std::vector<std::string>& tokens, size_t& i)
		{
			if (tokens[i] == "NOT")
			{
				//unary
				if (tokens[i + 1] == "VAR" && Environment::Exists(tokens[i + 2], Type::BOOL)) 
				{
					return { std::make_unique<VariableBool>(tokens[i + 2]) ,true,"success" };
				}
				else
				{
					i++;
					

				}
			}
		}

		

		inline static ParseResult ParseRelationalExpression(const std::vector<std::string>&tokens,size_t&i)
		{
			
		}

		inline static ParseResult ParseBinaryorValueExpression(const std::vector<std::string>& tokens, size_t& i)
		{
			static const std::unordered_set<std::string> operators = { "==","!=",">","<",">=","<=" };
			int foundcount = 0;
			for (int i = 0;i < 5 && i < tokens.size();i++)
			{
				
				if (operators.find(tokens[i]) != operators.end())
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
			static inline std::unordered_set<std::string> operators = { "+","-","*","/" };
			char c = ' ';
			ParseResult resultLeft = ParseValueExpression(tokens, i);
			if (!resultLeft.success) return resultLeft;
			if (dynamic_cast<NumericExpression*> (resultLeft.node.get())==nullptr)
			{
				return { nullptr,false,"binary expressions must have numeric values on both sides" };
			}
			if (operators.find(tokens[i]) == operators.end()) return { nullptr,false,"invalid binary expression operator" };

			char c = char(tokens[i][0]);
			i++;

			ParseResult resultRight = ParseValueExpression(tokens, i);
			if (!resultRight.success) return resultRight;
			if (dynamic_cast<NumericExpression*>(resultRight.node.get())==nullptr)
			{
				return { nullptr,false,"binary expressions must have numeric values on both sides" };
			}

			return { std::make_unique<BinaryExpression>(std::move(resultLeft.node),
				std::move(resultRight.node),c) };
			

			
		}

		inline static ParseResult ParseValueExpression(const std::vector<std::string>& tokens, size_t& i)
		{
			ParseResult result = { nullptr,false,"not executed" };
			if (tokens[i] == "VAR")
			{
				
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
	};
}
