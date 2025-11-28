#pragma once
#include "../Sysbase.h"
#include "../../../Events/EventManager.h"
#include "Tokens.h"
#include "../Physics/ColliderFunctions.h"
#define EXISTS_INTO_MAP(map,v)  map.find(v)!=map.end()
namespace D2Maker
{
    using Token = std::string;
    using TokenLine = std::vector<Token>;
    using TokenStream = std::vector<TokenLine>;
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
        static void LoadVarsFromJsonTemp(Json json);
        static void LoadVarsFromJsonData(Json json);
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

        static Number RetrieveNumber(const std::string& var);
        static Type RetrieveType(const std::string& var);
        static bool Exists(const std::string& var, const Type& type);
        static void InitializeTempVariables(TokenStream tokens);
        static Json WriteDataToJson();
        inline static bool ExistsGeneral(const std::string& var)
        {
            return 
                EXISTS_INTO_MAP(stringVars, var) ||
                EXISTS_INTO_MAP(intVars, var)    || 
                EXISTS_INTO_MAP(floatVars, var)  ||
                EXISTS_INTO_MAP(boolVars, var);
        }

        inline static void LoadVarsFromJson(Json data,VarLoadMode vlm)
        {
            (vlm == VarLoadMode::TEMP) ? LoadVarsFromJsonTemp(data) : LoadVarsFromJsonData(data);
        }

        
    };
}
