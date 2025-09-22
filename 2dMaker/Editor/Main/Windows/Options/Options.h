#pragma once
#include "Save.h"
#include "Resolution.h"
#include "General.h"

namespace D2Maker
{
	namespace GUI
	{
		class OptionsMenu
		{
		private:
			GUIAPI::ButtonWithCallback<> resolutionBtn;
			GUIAPI::ButtonWithCallback<> generalBtn;
			GUIAPI::ButtonWithCallback<> saveBtn;
			GUIAPI::ButtonWithCallback<> consoleBtn;
			SavePopup saveP;
			ResoulutionMenu resMenu;
			GeneralOptions generalOptions;

		public:
			OptionsMenu() : 
				resolutionBtn(585, 35, "Resolution", [this](void) 
					{
						resMenu.Activate();
					}
				),
				generalBtn(585, 35, "General", [this](void)
					{
						generalOptions.Activate();
					}
				),
				saveBtn(585, 35, "Save", [this](void)
					{
						FileSys::Save();
						saveP.Activate();
						CONSOLELOG("Project saved!");
					}
				),
				consoleBtn(585, 35, "Console", [this](void)
					{
						ConsoleShouldOpen::value = true;
					}
				)
			{ }

			void Update()
			{
				GUIAPI::GUIWindow::CreateFixedWindow(400, 410, 600, 190, "Options");
				ImGui::Text("Options");
				saveBtn.Update();
				generalBtn.Update();
				resolutionBtn.Update();
				consoleBtn.Update();
				saveP.Update();
				resMenu.Update();
				generalOptions.Update();
				GUIAPI::GUIWindow::EndWindow();
			}
		};
	}
}