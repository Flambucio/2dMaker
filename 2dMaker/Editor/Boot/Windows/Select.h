#pragma once
#include "Create.h"
#include <vector>
#include <string>

namespace D2Maker
{
	namespace GUI
	{
		class BootWindowSelect
		{
		private:
			GUIAPI::Menu menu;
			GUIAPI::ButtonWithCallback<> selectButton;
			GUIAPI::ButtonWithCallback<> createButton;
			GUIAPI::ButtonWithCallback<> deleteButton;
			BootPopUpCreate createPopUp;

		public:
			BootWindowSelect(std::vector<std::string> prjs);

			void Update();
		};
	}
}