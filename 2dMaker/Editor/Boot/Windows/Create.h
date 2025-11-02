#pragma once
#include "../../API/APIExport.h"

namespace D2Maker
{
	namespace GUI
	{
		class BootPopUpCreate
		{
		private:
			GUIAPI::PopUp popup;
			GUIAPI::TextField name;
			GUIAPI::ButtonWithCallback<> createBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;

		public:
			BootPopUpCreate();

			void Update();
			void Activate();
		};
	}
}