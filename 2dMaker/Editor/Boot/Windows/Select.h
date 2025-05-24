#pragma once
#include "Create.h"


namespace D2Maker
{
	namespace GUI
	{
		class BootWindowSelect
		{
		private:
			GUIAPI::Menu menu;
			GUIAPI::ButtonWithCallback<> selectButton;
			GUIAPI::ButtonWithCallback<> createButton;
			GUIAPI::ButtonWithCallback<> deleteButton;
			BootPopUpCreate createPopUp;
			
			
		public:
			
			BootWindowSelect(std::vector<std::string> prjs) : menu(prjs, 0, 1260, 30),
				selectButton(1280/3 -10, 40, "Select Project", [this](void) 
					{
						std::string value = menu.GetCurrentValue();
						FileSys::SelectProject(value);
					}
				),
				createButton(1280 / 3 -10, 40, "Create Project", [this](void)
					{
						createPopUp.Activate();
					}
				),
				deleteButton(1280 / 3 -10, 40, "Delete Project", [this](void)
					{
						std::string value = menu.GetCurrentValue();
						if (value != "")
						{
							FileSys::DeleteProject(value);
						}
						FileSys::GetProjects();
						std::vector<std::string> newValues;
						SetToVector<std::string>(FileSys::projectNames, newValues);
						menu.UpdateValues(newValues);
					}
				)
			{}

			void Update()
			{
				
				GUIAPI::GUIWindow::CreateFixedWindow(0, 0, 1280, 660, "Selezione");
				ImGui::Text("Select/Create/Delete a Project");
				menu.Update();
				GUIAPI::GUIWindow::EndWindow();

				GUIAPI::GUIWindow::CreateFixedWindow(0, 660, 1280, 60, "Pulsante");
				createButton.Update();
				ImGui::SameLine();
				selectButton.Update();
				ImGui::SameLine();
				deleteButton.Update();
				createPopUp.Update();
				GUIAPI::GUIWindow::EndWindow();


				


				

				
			}
		};
	}
}