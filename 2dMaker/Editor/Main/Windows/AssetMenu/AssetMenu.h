#pragma once
#include "../../../API/APIExport.h"

namespace D2Maker
{
	namespace GUI
	{
		class AssetMenu
		{
		private:
			const std::vector<std::string> dropdownValues = { "Textures","Audio" };
			GUIAPI::Dropdown dropdown{dropdownValues,0,"Asset type"};
		public:
			AssetMenu() = default;
			void Update()
			{
				GUIAPI::GUIWindow::CreateFixedWindow(1000, 60, 280, 60, "AssetMenuSelect");
				dropdown.Update();
				GUIAPI::GUIWindow::EndWindow();
				GUIAPI::GUIWindow::CreateFixedWindow(1000, 120, 280, 480, "AssetMenu");
				GUIAPI::GUIWindow::EndWindow();
			}
		};
	}
}