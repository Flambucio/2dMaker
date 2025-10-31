#pragma once
#include "../Sysbase.h"
#include "../../../Events/EventManager.h"
#include "Tokens.h"
#include "../Physics/ColliderFunctions.h"

namespace D2Maker
{
    using Number = std::variant<int, float>;
    enum class Type
    {
        STRING,
        INT,
        FLOAT,
        NUMERIC,
        BOOL,
        NUL
    };

    class Environment
    {
    private:
        template<typename T>
        static T RetrieveSpecific(std::unordered_map<std::string, T>& map, const std::string& var)
        {
            return map[var];
        }
    public:
        inline static std::unordered_map<std::string, std::string> stringVars = {};
        inline static std::unordered_map<std::string, int> intVars = {};
        inline static std::unordered_map<std::string, float> floatVars = {};
        inline static std::unordered_map<std::string, bool> boolVars = {};
        inline static std::unordered_map<std::string, Type> typeRegistry = {};


        template<typename T>
        static void Set(const std::string& var, const T& value)
        {
            if constexpr (std::is_same_v<T, std::string>)
            {
                stringVars[var] = value;
                typeRegistry[var] = Type::STRING;
            }
            else if constexpr (std::is_same_v<T, int>)
            {
                intVars[var] = value;
                typeRegistry[var] = Type::INT;
            }
            else if constexpr (std::is_same_v<T, float>)
            {
                floatVars[var] = value;
                typeRegistry[var] = Type::FLOAT;
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                boolVars[var] = value;
                typeRegistry[var] = Type::BOOL;
            }
            else
            {
                throw std::runtime_error("Unsupported type for variable: " + var);
            }
        }

        // Recupera una variabile (eccezione se non esiste)
        template<typename T>
        static T Retrieve(const std::string& var)
        {
            if constexpr (std::is_same_v<T, std::string>)
            {
                auto it = stringVars.find(var);
                if (it == stringVars.end()) throw std::runtime_error("Variable not found: " + var);
                return it->second;
            }
            else if constexpr (std::is_same_v<T, int>)
            {
                auto it = intVars.find(var);
                if (it == intVars.end()) throw std::runtime_error("Variable not found: " + var);
                return it->second;
            }
            else if constexpr (std::is_same_v<T, float>)
            {
                auto it = floatVars.find(var);
                if (it == floatVars.end()) throw std::runtime_error("Variable not found: " + var);
                return it->second;
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                auto it = boolVars.find(var);
                if (it == boolVars.end()) throw std::runtime_error("Variable not found: " + var);
                return it->second;
            }
            else
            {
                throw std::runtime_error("Unsupported type requested for variable: " + var);
            }

        }

        static Number RetrieveNumber(const std::string& var)
        {
            if (intVars.find(var) != intVars.end()) return intVars[var];
            if (floatVars.find(var) != floatVars.end()) return floatVars[var];
            return 0.0f;
        }

        static Type RetrieveType(const std::string& var)
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
}
