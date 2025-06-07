#pragma once
#include "../../Engine/EngineExport.h"

namespace D2Maker
{
	namespace GUI
	{
		namespace GUIAPI
		{
			class TextField
			{
			private:
				char buffer[128] = "";
				std::string text;
			public:
				TextField(std::string text)
				{
					this->text = text;
				}
				void Update()
				{
					ImGui::Text(text.c_str());
					ImGui::InputText(("##combo_"+text).c_str(), buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_AllowTabInput);
				}
				std::string GetText() const
				{
					return (std::string)(buffer);
				}
				void SetText(std::string str)
				{
					std::size_t len = std::min(str.size(), sizeof(buffer) - 1);
					std::memcpy(buffer, str.c_str(), len);
					buffer[len] = '\0';
				}
			};
		}
	}
}
