#pragma once
#include "ApiUtils.h"


namespace D2Maker
{
	namespace GUI
	{
		namespace GUIAPI
		{
			class Dropdown
			{
			private:
				int currentValueIndex = 0;
				std::vector<std::string> values;
				std::string label;
				std::string combo_id;

			public:
				inline Dropdown(const std::vector<std::string>& values, int defaultValueIndex, const std::string& label)
					: values(values), label(label)
				{
					currentValueIndex = APIUtils::GetValidIndex<std::string>(values,defaultValueIndex);
					combo_id = "##combo_" + label;
				}

				

				inline void DynamicUpdate(const std::vector<std::string>& newValues)
				{
					UpdateValues(newValues);
					Update();
				}

				inline std::string GetCurrVal() const
				{
					if (currentValueIndex < 0 || currentValueIndex >= static_cast<int>(values.size())) return "";
					return values[currentValueIndex];
				}

				inline int GetIndex() const
				{
					return currentValueIndex;
				}

				inline void UpdateValues(const std::vector<std::string>& newValues)
				{
					values = newValues;
					currentValueIndex = APIUtils::GetValidIndex<std::string>(values, currentValueIndex);
				}


				void SetCurrVal(std::string newVal);
				void Update();
			};
		}
	}
}

