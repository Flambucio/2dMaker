#pragma once
#include "Environment.h"


class Statement
{
public:
	uint32_t line = 0;
	Statement(uint32_t line) : line(line){}
};


class Expression
{
public:
	virtual ~Expression() = default;
	virtual int evaluateValue() { return 0; }

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
	T value;
	VariableExpression(std::string var)
	{
		
	}
};


