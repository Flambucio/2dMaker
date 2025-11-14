#pragma once
#include "../../../API/APIExport.h"


namespace D2Maker
{
	namespace GUI
	{
		class GeneralOptions
		{
		private:
			GUIAPI::ButtonWithCallback<> confirmBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			GUIAPI::PopUp popup;
			GUIAPI::TextField textBox_Image;
			GUIAPI::TextField textBox_Name;

		public:
			GeneralOptions();
			void Update();
			void ModifyOptions();
			void Activate();
		};
	}
}