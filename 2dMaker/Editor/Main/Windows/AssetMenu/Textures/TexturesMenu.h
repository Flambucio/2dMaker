#pragma once

#include "../../../../API/APIExport.h"

namespace D2Maker
{
	namespace GUI
	{
		class TexturesMenu
		{
		private:
			std::vector<std::string> textureNames = {};
			GUIAPI::Menu menu;
		public:
			TexturesMenu() : menu(textureNames,0,200,10){}


			
		};
	}
}
