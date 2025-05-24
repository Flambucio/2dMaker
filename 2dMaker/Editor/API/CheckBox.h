#pragma once
#include "../../Engine/EngineExport.h"

namespace D2Maker
{
	namespace GUI
	{
		namespace GUIAPI
		{
			class CheckBox
			{
			private:
				bool value;
				std::string text;
			public:
				CheckBox(std::string text, bool value = false)
				{
					this->text = text;
					this->value = value;
				}
				void Update()
				{
					ImGui::Checkbox(text.c_str(), &value);
				}
				bool GetValue() const
				{
					return value;
				}
			};
		}
	}
}
