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
			
			AssetMenu() = default;
			void Update();
			inline PreviewTextureWindow& GetPreviewWindowReference()
			{
				return tMenu.previewWindow;
			}
		};
	}
}