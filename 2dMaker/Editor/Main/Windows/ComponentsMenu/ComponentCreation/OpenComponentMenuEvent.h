#pragma once
#include <string>

//when using callbacks inside ComponentCreation.h the component windows dont open, so i will open them in
//ComponentMenuMain.h passing data with this struct

namespace D2Maker
{
	namespace GUI
	{
		struct OpenComponentMenuEventUI
		{
		
			 std::string component;
			 bool componentExists;
			 inline OpenComponentMenuEventUI(std::string component,bool componentExists) :
				 component(component), componentExists(componentExists) { }
		};
	}
}
