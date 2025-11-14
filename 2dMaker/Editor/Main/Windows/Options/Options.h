#pragma once
#include "Save.h"
#include "Resolution.h"
#include "General.h"

namespace D2Maker
{
	namespace GUI
	{
		class OptionsMenu
		{
		private:
			GUIAPI::ButtonWithCallback<> resolutionBtn;
			GUIAPI::ButtonWithCallback<> generalBtn;
			GUIAPI::ButtonWithCallback<> saveBtn;
			GUIAPI::ButtonWithCallback<> consoleBtn;
			SavePopup saveP;
			ResoulutionMenu resMenu;
			GeneralOptions generalOptions;

		public:
			OptionsMenu();
			void Update();
		};
	}
}