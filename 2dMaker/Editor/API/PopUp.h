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
				inline PopUp(std::string id)
				{
					this->id = id;
				}
				inline void Open()
				{
					ImGui::OpenPopup(id.c_str());
					open = true;
				}
				inline void Close()
				{
					ImGui::CloseCurrentPopup();
					open = false;
				}
				inline std::string GetId()
				{
					return id;
				}

				inline bool Begin()
				{
					bool value = ImGui::BeginPopupModal(id.c_str(),
						&open, ImGuiWindowFlags_AlwaysAutoResize);
					return value;
				}
				inline void End()
				{
					ImGui::EndPopup();
				}



			};
		}

	}
}