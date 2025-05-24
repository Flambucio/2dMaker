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
				Menu(std::vector<std::string>values,int defaultValueIndex,int bWidth,int bHeight)
				{
					this->values = values;
					this->currentValueIndex = APIUtils::GetValidIndex<std::string>(values, defaultValueIndex);
					this->bHeight = bHeight;
					this->bWidth = bWidth;

				}

				void Update()
				{
					for (int i = 0;i < values.size();i++)
					{
						bool isSelected = (i == currentValueIndex);

						if (!isSelected)
						{
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
						}

						if (ImGui::Button(values[i].c_str(), ImVec2(bWidth, bHeight)))
						{
							currentValueIndex = i;
						}

						if (!isSelected)
						{
							ImGui::PopStyleColor(3);
						}
					}
				}

				void DynamicUpdate(std::vector<std::string> newValues)
				{
					values = newValues;
					currentValueIndex = APIUtils::GetValidIndex<std::string>(values, currentValueIndex);
					Update();
				}

				std::string GetCurrentValue()
				{
					if (currentValueIndex < 0 || currentValueIndex >= static_cast<int>(values.size())) return "";
					return values[currentValueIndex];
				}

				void UpdateValues(std::vector<std::string> newValues)
				{
					values = newValues;
					currentValueIndex = APIUtils::GetValidIndex<std::string>(values, currentValueIndex);
				}
			};
		}
	}
}
