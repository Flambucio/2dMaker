#pragma once
#include "Textures/TexturesMenu.h"
#include "Audios/AudioMenu.h"

namespace D2Maker
{
	namespace GUI
	{
		class AssetMenu
		{
		private:
			const std::vector<std::string> dropdownValues = { "Textures","Audio" };
			GUIAPI::Dropdown dropdown{dropdownValues,0,"Asset type"};
			TexturesMenu tMenu;
			AudioMenu aMenu;
			bool loadedAssetsStart=false;
		public:
			
			AssetMenu()
			{

			};
			void Update()
			{
				if (!loadedAssetsStart)
				{
					aMenu.LoadAudioNames();
					tMenu.LoadTextureNames();
					loadedAssetsStart = true;
				}
				GUIAPI::GUIWindow::CreateFixedWindow(1000, 60, 280, 60, "AssetMenuSelect");
				dropdown.Update();
				GUIAPI::GUIWindow::EndWindow();
				if (dropdown.GetCurrVal() == dropdownValues[0])
				{
					tMenu.Update();
				}
				else if (dropdown.GetCurrVal() == dropdownValues[1])
				{
					aMenu.Update();
				}

			}

			PreviewTextureWindow& GetPreviewWindowReference()
			{
				return tMenu.previewWindow;
			}
		};
	}
}