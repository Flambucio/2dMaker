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
				int currentValueIndex = 0;
				std::vector<std::string> values;
				std::string label;
				std::string combo_id;

			public:
				Dropdown(const std::vector<std::string>& values, int defaultValueIndex, const std::string& label)
					: values(values), label(label)
				{
					currentValueIndex = GetValidIndex(defaultValueIndex);
					combo_id = "##combo_" + label;
				}

				void Update()
				{
					if (currentValueIndex == -1 || values.empty()) return;

					ImGui::Text("%s", label.c_str());

					if (ImGui::BeginCombo(combo_id.c_str(), values[currentValueIndex].c_str()))
					{
						for (int i = 0; i < values.size(); ++i)
						{
							bool selected = (currentValueIndex == i);
							if (ImGui::Selectable(values[i].c_str(), selected))
								currentValueIndex = i;

							if (selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}
				}

				void DynamicUpdate(const std::vector<std::string>& newValues, int defaultValueIndex = 0)
				{
					values = newValues;
					currentValueIndex = GetValidIndex(defaultValueIndex);
					Update();
				}

				std::string GetCurrVal() const
				{
					if (currentValueIndex < 0 || currentValueIndex >= static_cast<int>(values.size())) return "";
					return values[currentValueIndex];
				}

				int GetIndex() const
				{
					return currentValueIndex;
				}

			private:
				int GetValidIndex(int index) const
				{
					if (values.empty()) return -1;
					if (index < 0 || index >= static_cast<int>(values.size())) return 0;
					return index;
				}
			};
		}
	}
}

