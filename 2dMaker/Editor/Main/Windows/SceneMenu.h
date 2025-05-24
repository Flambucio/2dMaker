#pragma once
#include "../../API/APIExport.h"

namespace D2Maker
{
	namespace GUI
	{
		class SceneMenu
		{
		private:

			GLFWwindow*& window;
			GUIAPI::Dropdown scenesDropdown;
			GUIAPI::Dropdown noneDropdown;
			GUIAPI::ButtonWithCallback<> createScene;
			GUIAPI::ButtonWithCallback<> deleteBtn;
			GUIAPI::ButtonWithCallback<> setDefault;
			std::vector<std::string> sceneNames;
		public:
			SceneMenu(GLFWwindow*& window) : window(window),scenesDropdown({}, 0, "Current Scene"),
				noneDropdown({ "(None)" }, 0, "Current Scene"),
				createScene(130, 20, "Create Scene", [this](void)
					{

					}
				),

				deleteBtn(130, 20, "Delete Scene", [this](void)
					{

					}
				),
				setDefault(150, 20, "Set Default Scene", [this](void)
					{

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
				GUIAPI::GUIWindow::EndWindow();
			}

			void LoadStartingSceneNames()
			{
				for (auto& element : SceneManager::scenes)
				{
					sceneNames.push_back(element.first);

				}
				scenesDropdown.UpdateValues(sceneNames);
			}

		};
	}
}

