#pragma once
#include "../../API/APIExport.h"

namespace D2Maker
{
	namespace GUI
	{

		class BootPopUpCreate
		{
		private:
			GUIAPI::PopUp popup;
			GUIAPI::TextField name;
			GUIAPI::ButtonWithCallback<> createBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
		public:
			BootPopUpCreate() : popup("Create Project Menu"), name("Project name"),
				createBtn(150, 30,"Create Project", [this]()
					{
						if (FileSys::CreateProject(name.GetText()))
						{
							FileSys::SelectProject(name.GetText());
						}
					}
				),
				closeBtn(150, 30, "Close", [this]()
					{
						ImGui::CloseCurrentPopup();
					}
				)
			{
			}

			void Update()
			{
				ImGui::SetNextWindowSize(ImVec2(350, 120), ImGuiCond_Always);
				if (popup.Begin())
				{
					name.Update();
					createBtn.Update();
					ImGui::SameLine();
					closeBtn.Update();
					popup.End();
				}
				
			}
			void Activate()
			{
				popup.Open();
			}

		};
	}
}
