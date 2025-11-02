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
				inline CheckBox(std::string text, bool value = false)
				{
					this->text = text;
					this->value = value;
				}
				inline void Update()
				{
					ImGui::Checkbox(text.c_str(), &value);
				}
				inline bool GetValue() const
				{
					return value;
				}
				inline void SetValue(bool value)
				{
					this->value = value;
				}
			};
		}
	}
}
