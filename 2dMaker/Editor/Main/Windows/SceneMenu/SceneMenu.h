#pragma once
#include "SceneMenuCreatePopup.h"

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
		public:
			GUIAPI::Dropdown scenesDropdown;
			std::vector<std::string> sceneNames;
			SceneMenu(GLFWwindow*& window) : window(window), scenesDropdown({}, 0, "Current Scene"),
				noneDropdown({ "(None)" }, 0, "Current Scene"),
				createScene(130, 20, "Create Scene", [this](void)
					{
						SMCPopup.Activate();
					}
				),

				deleteBtn(130, 20, "Delete Scene", [this](void)
					{
						std::string toRemove = SceneManager::currentScene;
						SceneManager::RemoveScene(toRemove);
						if (!sceneNames.empty())
						{
							LoadSceneNames();
						}
						else
						{
							sceneNames = {};
						}
						
						std::string nextScene;
						!sceneNames.empty() ? nextScene = sceneNames[0] : nextScene = "";
						SceneManager::SelectScene(nextScene);
						scenesDropdown.SetCurrVal(nextScene);
						if (SceneManager::defaultScene == toRemove) SceneManager::defaultScene = nextScene;
					}
				),
				setDefault(150, 20, "Set Default Scene", [this](void)
					{
						SceneManager::defaultScene = SceneManager::currentScene;
					}
				),
				SMCPopup([this](void)
					{
						LoadSceneNames();
					}
				)
				
			{

			}

			void Update()
			{
				GUIAPI::GUIWindow::CreateFixedWindow(0, 0, 1280, 60, "sceneWindow");
				
				if (sceneNames.empty())
				{
					noneDropdown.Update();
				}
				else
				{
					scenesDropdown.Update();
				}
				
				ImGui::SameLine();
				createScene.Update();
				ImGui::SameLine();
				deleteBtn.Update();
				ImGui::SameLine();
				setDefault.Update();
				SMCPopup.Update();
				GUIAPI::GUIWindow::EndWindow();
				std::string sceneName = scenesDropdown.GetCurrVal();
				if (sceneName != SceneManager::currentScene)
				{
					SceneManager::SelectScene(sceneName);
					
				}
			}

			void LoadSceneNames()
			{
				sceneNames.clear();
				for (auto& element : SceneManager::scenes)
				{
					sceneNames.push_back(element.first);

				}
				scenesDropdown.UpdateValues(sceneNames);
			}

		};
	}
}

