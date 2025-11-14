#pragma once
#include "../../../../API/APIExport.h"

namespace D2Maker
{
	namespace GUI
	{
		class TextureDelete
		{
		private:
			GUIAPI::ButtonWithCallback<> confirmBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			GUIAPI::PopUp popup;
			std::function<void()> deleteTexture;
			
		public:
			TextureDelete(std::function<void()> deleteTexture);
			void Update();
			inline void Activate()
			{
				popup.Open();
			}
		};
	}
}
