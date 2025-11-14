#pragma once
#include "../../../API/APIExport.h"

namespace D2Maker
{
	namespace GUI
	{
		class EntityCreatePopup
		{
		private:
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> close;
			GUIAPI::ButtonWithCallback<> create;
			GUIAPI::TextField textBox;
			GUIAPI::CheckBox checkbox;
		public:
			EntityCreatePopup();
			void Update();
			inline void Activate()
			{
				popup.Open();
			}
		};
	}
}