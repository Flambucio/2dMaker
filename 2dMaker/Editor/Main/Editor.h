#pragma once
#include "Windows/SceneMenu/SceneMenu.h"
#include "Windows/GameControl/GameControl.h"
#include "Windows/EntityMenu/EntityMenu.h"

namespace D2Maker
{
	namespace GUI
	{
		class Editor
		{
		private:
			GLFWwindow*& guiWindow;
			Entity selectedEntity;
			SceneMenu sceneMenu;
			GameControls gameControls;
			EntityMenu entityMenu;
		public:
			bool runGameFlag = false;
			Editor(GLFWwindow*& guiWindow) : guiWindow(guiWindow),sceneMenu(guiWindow),gameControls(runGameFlag)
			{

			}


			void Update()
			{
				glfwMakeContextCurrent(guiWindow);
				GUIAPI::GUIWindow::StartFrame();

				sceneMenu.Update();
				entityMenu.Update();
				gameControls.Update();




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

