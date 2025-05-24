#pragma once
#include "../../Engine/EngineExport.h"

namespace D2Maker
{
	namespace GUI
	{
		namespace GUIAPI
		{
			class PopUp
			{
			private:
				std::string id;
				bool open = false;
				
			public:
				PopUp(std::string id)
				{
					this->id = id;
				}
				void Open()
				{
					ImGui::OpenPopup(id.c_str());
					open = true;
				}
				void Close()
				{
					ImGui::CloseCurrentPopup();
					open = false;
				}
				std::string GetId()
				{
					return id;
				}

				bool Begin()
				{
					bool value = ImGui::BeginPopupModal(id.c_str(),
						&open, ImGuiWindowFlags_AlwaysAutoResize);
					return value;
				}
				void End()
				{
					ImGui::EndPopup();
				}


			};
		}

	}
}