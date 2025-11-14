#pragma once
#include "../../../API/APIExport.h"
namespace D2Maker
{
	namespace GUI
	{
		class ResoulutionMenu
		{
		private:
			const std::vector<std::string> dropdownOptions = { "16/9","1/1","9/16" };
			GUIAPI::Dropdown dropdown;
			
			GUIAPI::TextField textBox_Width;
			GUIAPI::TextField textBox_VirtualWidth;
			GUIAPI::ButtonWithCallback<> closeBtn;
			GUIAPI::ButtonWithCallback<> confirmBtn;
			GUIAPI::CheckBox fullscreenBox;
			GUIAPI::PopUp popup;
		public:
			ResoulutionMenu();
			void Update();
			void ModifyResolution();
			void Activate();
		}; 
	}
}
