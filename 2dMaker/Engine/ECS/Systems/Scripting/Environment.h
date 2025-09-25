#pragma once
#include "../Sysbase.h"
#include "../../../Events/EventManager.h"
#include "Tokens.h"
#include "../Physics/ColliderFunctions.h"

enum Type
{
	STRING,
	INT,
	FLOAT,
	BOOL,
	NUL
};

class Environment
{
private:

public:
	inline static std::unordered_map<std::string, std::string> stringVars = {};
	inline static std::unordered_map<std::string, int> intVars = {};
	inline static std::unordered_map<std::string, float> floatVars = {};
	inline static std::unordered_map<std::string, bool> boolVars = {};
	inline static std::unordered_map<std::string, Type> typeRegistry = {};

	template<typename T>
	static Type RetrieveType(const std::string &var)
	{
		Type type = Type::NUL;
		auto it = typeRegistry.find(var);
		if (it == typeRegistry.end()) return Type::NUL;
		type = typeRegistry[var];
		return type;
	}

	static bool Exists(const std::string& var, const Type& type)
	{
		switch (type)
		{
		case Type::STRING:
			return stringVars.find(var) != stringVars.end();
		case Type::INT:
			return intVars.find(var) != intVars.end();
		case Type::FLOAT:
			return floatVars.find(var) != floatVars.end();
		case Type::BOOL:
			return boolVars.find(var) != boolVars.end();
		case Type::NUL:
			return false;
		}
	}

	
};