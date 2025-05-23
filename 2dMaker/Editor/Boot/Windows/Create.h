#pragma once
#include "../../API/APIExport.h"


namespace D2Maker
{
	namespace GUI
	{
		class APIBootWindowCreate
		{
		private:
			GUIAPI::Menu menu;
		public:
			
			APIBootWindowCreate(std::vector<std::string> prjs) : menu(prjs, 0, 100, 200, 1050, 30)
			{	}

			void Update()
			{
				GUIAPI::GUIWindow::CreateFixedWindow(0, 0, 1100, 660, "Selezione");
				menu.Update();
				GUIAPI::GUIWindow::EndWindow();
				GUIAPI::GUIWindow::CreateFixedWindow(0, 660, 1100, 60, "Pulsante");
				if (ImGui::Button("Select Project", ImVec2(1050, 40)))
				{
					std::string value = menu.GetCurrentValue();
					FileSys::SelectProject(value);
				}
				GUIAPI::GUIWindow::EndWindow();
			}
		};
	}
}