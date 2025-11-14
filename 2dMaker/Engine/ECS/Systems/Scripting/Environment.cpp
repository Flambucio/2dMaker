#include "Environment.h"
#include "Statements.h"
using namespace D2Maker;

void Environment::LoadVarsFromJsonTemp(Json json)
{
    for (auto& [key, value] : json.items())
    {
        if (ExistsGeneral(key)) continue;
        if (value == "int") Set<int>(key, 0);
        else if (value == "float") Set<float>(key, 0.0f);
        else if (value == "string") Set<std::string>(key, "");
        else if (value == "bool") Set<bool>(key, false);
        CONSOLELOG("[WARN]: variable " + key + " has invalid type");
    }
}
void Environment::LoadVarsFromJsonData(Json json)
{
    for (auto& [key, value] : json.items())
    {
        if (ExistsGeneral(key)) continue;
        if (!value.contains("type") && !value.contains("value")) continue;
        const std::string type = value["type"];
        if (type == "int" && value["value"].is_number_integer()) SetDataVar<int>(key, value["value"].get<int>());
        else if (type == "float" && value["value"].is_number_float()) SetDataVar<float>(key, value["value"].get<float>());
        else if (type == "string" && value["value"].is_string()) SetDataVar<std::string>(key, value["value"].get<std::string>());
        else if (type == "bool" && value["value"].is_boolean()) SetDataVar<bool>(key, value["value"].get<bool>());

    }
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