#pragma once
#include "../../../API/APIExport.h"

namespace D2Maker
{
	namespace GUI
	{
		class EntityModifyPopup
		{
		private:
			GUIAPI::TextField textBox;
			GUIAPI::ButtonWithCallback<> closeBtn;
			GUIAPI::ButtonWithCallback<> confirmBtn;
			GUIAPI::PopUp popup;
			std::string currentEntityName = "";
		public:
			EntityModifyPopup();
			void Update();

			inline void Activate(std::string currentEntityName)
			{
				this->currentEntityName = currentEntityName;
				popup.Open();
			}

		};
	}
}
