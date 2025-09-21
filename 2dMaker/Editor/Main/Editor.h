#pragma once
#include "Windows/SceneMenu/SceneMenu.h"
#include "Windows/GameControl/GameControl.h"
#include "Windows/EntityMenu/EntityMenu.h"
#include "Windows/ComponentsMenu/ComponentsMenuMain.h"
#include "Windows/AssetMenu/AssetMenu.h"
#include "Windows/Options/Options.h"
#include "../../Engine/common/Console.h"
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
			AssetMenu assetMenu;
			OptionsMenu optionsMenu;
			Console console;
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
				assetMenu.Update();
				optionsMenu.Update();
				console.Update();


				GUIAPI::GUIWindow::EndFrame(guiWindow);
			}

			void LoadStartingScenesNames()
			{
				sceneMenu.LoadSceneNames();
				sceneMenu.scenesDropdown.SetCurrVal(SceneManager::defaultScene);
			}

			PreviewTextureWindow& GetPreviewWindowRef()
			{
				return assetMenu.GetPreviewWindowReference();
			}

		};
	}
}

