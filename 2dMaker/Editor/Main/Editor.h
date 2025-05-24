#pragma once
#include "Windows/SceneMenu.h"

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
		public:
			Editor(GLFWwindow*& guiWindow) : guiWindow(guiWindow),sceneMenu(guiWindow)
			{
				
			}


			void Update()
			{
				glfwMakeContextCurrent(guiWindow);
				GUIAPI::GUIWindow::StartFrame();

				sceneMenu.Update();




				GUIAPI::GUIWindow::EndFrame(guiWindow);
			}

			void LoadStartingScenesNames()
			{
				sceneMenu.LoadStartingSceneNames();
			}

		};
	}
}

