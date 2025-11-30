#include "Environment.h"
#include "Statements.h"
using namespace D2Maker;

void Environment::LoadVarsFromJsonTemp(Json json)
{
    for (auto& [key, value] : json.items())
    {
        if (ExistsGeneral(key) || !value.is_string()) continue;
        if (value == "int") Set<int>(key, 0);
        else if (value == "float") Set<float>(key, 0.0f);
        else if (value == "string") Set<std::string>(key, "");
        else if (value == "bool") Set<bool>(key, false);
        else CONSOLELOG("[WARN]: variable " + key + " has invalid type");
        
    }
}
void Environment::InitializeTempVariables(TokenStream tokens)
{
    for (int i = 0;i < tokens.size() - 1;i++)
    {
        if (tokens[i].size() != 2) continue;
        if (ExistsGeneral(tokens[i][0]))
        {
            Token var = tokens[i][0];
            Token value = tokens[i][1];
            Type varT = RetrieveType(var);
            Type valueT = Type::NUL;
            if (IsInt(value)) valueT = Type::INT;
            else if (IsFloat(value)) valueT = Type::FLOAT;
            else if (value == "true" || value == "false") valueT = Type::BOOL;
            else valueT = Type::STRING;

            if (varT != valueT) continue;
            int n = 0;
            float f = 0;
            bool b = false;
            switch (varT)
            {
                
                case Type::INT: 
                    
                    ConvertStringToNum<int>(value, n);
                    Set<int>(var,n);
                    break;
                case Type::FLOAT:
                   
                    ConvertStringToNum<float>(std::string(value), f);
                    Set<float>(var, f);
                    break;
                case Type::BOOL:
                    b = value == "true" ? true : false;
                    Set<bool>(var, b);
                    break;
                case Type::STRING:
                    Set<std::string>(var, value);
                    break;
            }


        }
    }
}
void Environment::LoadVarsFromJsonData(Json json)
{
    for (auto& [key, value] : json.items())
    {
        TRACE("SetDataVar called for: " + key);
        if (ExistsGeneral(key))
        {
            CONSOLELOG(key + " already exists (data)");
            continue;

        }
        
        if (!value.contains("type") || !value.contains("value"))
        {
            CONSOLELOG(key + " doesnt contain the necessary data");
            continue;
        }
        const std::string type = value["type"];
        if (type == "int" && value["value"].is_number_integer()) SetDataVar<int>(key, value["value"].get<int>());
        else if (type == "float" && value["value"].is_number_float()) SetDataVar<float>(key, value["value"].get<float>());
        else if (type == "string" && value["value"].is_string()) SetDataVar<std::string>(key, value["value"].get<std::string>());
        else if (type == "bool" && value["value"].is_boolean()) SetDataVar<bool>(key, value["value"].get<bool>());
        else CONSOLELOG("not matching types in " + key);

        CONSOLELOG("parsing data var");
        TRACE("parsing data var");

    }
    std::string log="";
    for (const std::string& var : varsToSave)
    {
        log += var + "|";
    }
    TRACE(log);
}

Environment::Json Environment::WriteDataToJson()
{
    Json json = Json::object();

    

    for (const auto& var : varsToSave)
    {
        
        TRACE("Saving var: " + var);
        /*
        if (typeRegistry.find(var) == typeRegistry.end())
            TRACE("typeRegistry MISSING for " + var);

        if (intVars.find(var) == intVars.end() &&
            floatVars.find(var) == floatVars.end() &&
            boolVars.find(var) == boolVars.end() &&
            stringVars.find(var) == stringVars.end())
            TRACE("No map contains variable " + var);
            */
        Type t = RetrieveType(var);
        if (Type::NUL == t) TRACE("invalid type during writing datavar");
        Json entry = Json::object();

        switch (t) 
        {
        case Type::INT:
            if (intVars.find(var) != intVars.end())
            {
                entry["type"] = "int";
                entry["value"] = intVars[var];
            }
            break;
        case Type::FLOAT:
            if (floatVars.find(var) != floatVars.end()) 
            {
                entry["type"] = "float";
                entry["value"] = floatVars[var];
            }
            break;
        case Type::BOOL:
            if (boolVars.find(var) != boolVars.end())
            {
                entry["type"] = "bool";
                entry["value"] = boolVars[var];
            }
            break;
        case Type::STRING:
            if (stringVars.find(var) != stringVars.end())
            {
                entry["type"] = "string";
                entry["value"] = stringVars[var];
            }
            break;
        }

        if (!entry.empty()) json[var] = entry;
    }

    return json;
}


Number Environment::RetrieveNumber(const std::string& var)
{
    if (intVars.find(var) != intVars.end()) return intVars[var];
    if (floatVars.find(var) != floatVars.end()) return floatVars[var];
    return 0.0f;
}

Type Environment::RetrieveType(const std::string& var)
{
    Type type = Type::NUL;
    auto it = typeRegistry.find(var);
    if (it == typeRegistry.end()) return Type::NUL;
    type = typeRegistry[var];
    return type;
}


bool Environment::Exists(const std::string& var, const Type& type)
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