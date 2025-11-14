#pragma once
#include "../../../../API/APIExport.h"

namespace D2Maker
{
	namespace GUI
	{
		class TextureAdding
		{
		private:
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			GUIAPI::TextField textBox_path;
			GUIAPI::TextField textBox_name;
			std::function<void()> updateList;
		public:
			TextureAdding(std::function<void()> updateList);
			void Update();
			inline void Activate()
			{
				popup.Open();
			}
			bool AddTexture();

		};
	}
}