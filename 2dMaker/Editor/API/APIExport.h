#pragma once
#include "dropdown.h"
#include "Menu.h"
#include "WindowGUI.h"
#include "CheckBox.h"
#include "TextField.h"
#include "PopUp.h"
#include "ButtonWithCallback.h"

#define CONSOLELOGCOMPONENTSUCCESS(x,y) std::string c = std::type_index(typeid(x)).name();\
CONSOLELOG("Succesfully created component: "+ c + "to entity: "+ std::to_string(y));

#define CONSOLELOGCOMPONENTSUCCESSMODIFY(x,y) std::string c = std::type_index(typeid(x)).name();\
CONSOLELOG("Succesfully modified component: "+ c + "from entity: "+ std::to_string(y));

#define CONSOLELOGCOMPONENTUNSUCCESFUL(x,y) std::string c = std::type_index(typeid(x)).name();\
CONSOLELOG("Error adding/modifying component: "+ c + "to entity: "+ std::to_string(y) );\
CONSOLELOG("Check dependencies or if the component already exists");