#pragma once
#include "SceneMenuCreatePopup.h"
#include "SceneMenuDeletePopup.h"

namespace D2Maker
{
	namespace GUI
	{
		class SceneMenu
		{
		private:

			GLFWwindow*& window;
			
			GUIAPI::Dropdown noneDropdown;
			GUIAPI::ButtonWithCallback<> createScene;
			GUIAPI::ButtonWithCallback<> deleteBtn;
			GUIAPI::ButtonWithCallback<> setDefault;
			SceneMenuCreatePopup SMCPopup;
			SceneMenuDeletePopup SMDPopup;
			
		public:
			GUIAPI::Dropdown scenesDropdown;
			std::vector<std::string> sceneNames;
			SceneMenu(GLFWwindow*& window);
			void Update();
			void LoadSceneNames();
			void DeleteFunc();
		};
	}
}

