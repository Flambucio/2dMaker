#pragma once
#include "Windows/SceneMenu/SceneMenu.h"
#include "Windows/GameControl/GameControl.h"
#include "Windows/EntityMenu/EntityMenu.h"
#include "Windows/ComponentsMenu/ComponentsMenuMain.h"

namespace D2Maker
{
	namespace GUI
	{
		class Editor
		{
		private:
			GLFWwindow*& guiWindow;
			Entity selectedEntity=0;
			SceneMenu sceneMenu;
			GameControls gameControls;
			EntityMenu entityMenu;
			ComponentsMenu componentsMenu;
		public:
			bool runGameFlag = false;
			Editor(GLFWwindow*& guiWindow) : guiWindow(guiWindow),sceneMenu(guiWindow),gameControls(runGameFlag),
				componentsMenu(entityMenu.GetSelectedEntity(),entityMenu.GetEntityBuffer())
			{
				//componentsMenu.LoadStartingComponentsNames();
			}


			void Update()
			{
				glfwMakeContextCurrent(guiWindow);
				GUIAPI::GUIWindow::StartFrame();

				sceneMenu.Update();
				entityMenu.Update();
				gameControls.Update();
				componentsMenu.Update();




				GUIAPI::GUIWindow::EndFrame(guiWindow);
			}

			void LoadStartingScenesNames()
			{
				sceneMenu.LoadSceneNames();
				sceneMenu.scenesDropdown.SetCurrVal(SceneManager::defaultScene);
			}

		};
	}
}

