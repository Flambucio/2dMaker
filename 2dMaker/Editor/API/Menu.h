#pragma once
#include "ApiUtils.h"

namespace D2Maker
{
	namespace GUI
	{
		namespace GUIAPI
		{
			class Menu
			{
			public:
				std::vector<std::string> values;
				int currentValueIndex = 0;
				int bWidth = 0;
				int bHeight = 0;
				inline Menu(std::vector<std::string>values,int defaultValueIndex,int bWidth,int bHeight)
				{
					this->values = values;
					this->currentValueIndex = APIUtils::GetValidIndex<std::string>(values, defaultValueIndex);
					this->bHeight = bHeight;
					this->bWidth = bWidth;

				}

				void Update();

				inline void DynamicUpdate(std::vector<std::string> newValues)
				{
					values = newValues;
					currentValueIndex = APIUtils::GetValidIndex<std::string>(values, currentValueIndex);
					Update();
				}

				inline std::string GetCurrentValue()
				{
					if (currentValueIndex < 0 || currentValueIndex >= static_cast<int>(values.size())) return "";
					return values[currentValueIndex];
				}

				inline void UpdateValues(std::vector<std::string> newValues)
				{
					values = newValues;
					currentValueIndex = APIUtils::GetValidIndex<std::string>(values, currentValueIndex);
				}
			};
		}
	}
}
