#pragma once
#include "../Sysbase.h"
#include "../../../Events/EventManager.h"
#include "Tokens.h"
#include "../Physics/ColliderFunctions.h"
#define EXISTS_INTO_MAP(map,v)  map.find(v)!=map.end()
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

    enum class VarLoadMode
    {
        TEMP, //temporary variables
        DATA // variables that shoul be saved when apps closes
    };

    class Environment
    {
        

        template<typename T>
        using VarMap = std::unordered_map<std::string, T>;

        using TypeRegistry = std::unordered_map<std::string, Type>;
        using SaveRegistry = std::vector<std::string>;
        using Json = nlohmann::json;
    private:
        template<typename T>
        static T RetrieveSpecific(std::unordered_map<std::string, T>& map, const std::string& var)
        {
            return map[var];
        }
        static void LoadVarsFromJsonTemp(Json json)
        {
            for (auto& [key, value] : json.items())
            {
                if (ExistsGeneral(key)) continue;
                if (value == "int") Set<int>(key, 0);
                else if (value == "float") Set<float>(key,0.0f);
                else if (value == "string") Set<std::string>(key,"");
                else if (value == "bool") Set<bool>(key,false);
                CONSOLELOG("[WARN]: variable " + key + " has invalid type");
            }
        }
        static void LoadVarsFromJsonData(Json json)
        {
            for (auto& [key, value] : json.items())
            {
                if (ExistsGeneral(key)) continue;
                if (!value.contains("type") && !value.contains("value")) continue;
                const std::string type = value["type"];
                if (type == "int" && value["value"].is_number_integer()) SetDataVar<int>(key, value["value"].get<int>());
                else if(type=="float"&&value["value"].is_number_float()) SetDataVar<float>(key, value["value"].get<float>());
                else if (type == "string" && value["value"].is_string()) SetDataVar<std::string>(key, value["value"].get<std::string>());
                else if (type == "bool" && value["value"].is_boolean()) SetDataVar<bool>(key, value["value"].get<bool>());

            }
        }
        
    public:
        inline static VarMap<std::string> stringVars = {};
        inline static VarMap<int> intVars = {};
        inline static VarMap<float> floatVars = {};
        inline static VarMap<bool> boolVars = {};
        inline static SaveRegistry varsToSave = {};
        inline static TypeRegistry typeRegistry = {};


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

        template<typename T>
        static void SetDataVar(const std::string& var, const T& value)
        {
            varsToSave.push_back(var);
            Set<T>(var, value);
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



        inline static bool Exists(const std::string& var, const Type& type)
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

        inline static bool ExistsGeneral(const std::string& var)
        {
            return 
                EXISTS_INTO_MAP(stringVars, var) ||
                EXISTS_INTO_MAP(intVars, var)    || 
                EXISTS_INTO_MAP(floatVars, var)  ||
                EXISTS_INTO_MAP(boolVars, var);
        }

        static void LoadVarsFromJson(Json data,VarLoadMode vlm)
        {
            (vlm == VarLoadMode::TEMP) ? LoadVarsFromJsonTemp(data) : LoadVarsFromJsonData(data);
        }

        
    };
}
