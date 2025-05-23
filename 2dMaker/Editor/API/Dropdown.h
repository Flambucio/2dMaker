#pragma once
#include "../../Engine/EngineExport.h"

namespace D2Maker
{
	namespace GUI
	{
		namespace GUIAPI
		{
			class Dropdown
			{
			private:
				std::string currentValue;
				std::vector<std::string> values;
				std::string text;
			public:
				Dropdown(std::vector<std::string> values,std::string defaultValue,std::string text)
				{

				}
				void Update()
				{

				}

				std::string GetCurrVal() const
				{
					return currentValue;
				}
			};
		}
	}
}
