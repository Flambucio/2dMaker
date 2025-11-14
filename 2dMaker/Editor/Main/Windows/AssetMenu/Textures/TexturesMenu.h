#pragma once

#include "TextureAdd.h"
#include "TextureDelete.h"

namespace D2Maker
{
	namespace GUI
	{
		class TexturesMenu
		{
		private:
			std::vector<std::string> textureNames = {};
			GUIAPI::Menu menu;
			GUIAPI::ButtonWithCallback<> deleteBtn;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> previewButton;
			TextureAdding tAdd;
			TextureDelete tDel;
			
		public:
			D2Maker::PreviewTextureWindow previewWindow;
			TexturesMenu();
			void LoadTextureNames();
			void Update();
			void DeleteCallback();
			
		};
	}
}
