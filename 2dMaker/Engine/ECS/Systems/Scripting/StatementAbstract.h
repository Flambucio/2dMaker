#pragma once
#include "../../../common/Core.h"

class ASTNode
{
public:
	virtual ~ASTNode() = default;
};

class Statement : public ASTNode
{
public:
	uint32_t line = 0;
	inline Statement(uint32_t line) : line(line) {}
	virtual void Execute() = 0;
};