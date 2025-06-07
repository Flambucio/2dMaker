#pragma once
#include "../../../../API/APIExport.h"
namespace D2Maker
{
	namespace GUI
	{
		class AudioComponentCreator
		{
		private:
			Entity& selectedEntity;
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			std::function<void()> updateComponents;
			GUIAPI::TextField textBox;
			bool componentExists = false;
		public:
			AudioComponentCreator(Entity& selectedEntity, std::function<void()> closeCreation) : updateComponents(updateComponents),
				popup("Audio"),
				selectedEntity(selectedEntity),
				textBox("Audio Name"),
				addBtn(100, 30, "Add/Mod", [this](void)
					{
						AddAudioComponent();
					}
				),
				closeBtn(100, 30, "Close", [this](void)
					{
						popup.Close();
					}
				)
			{
			}

			void Update()
			{
				if (popup.Begin())
				{
					textBox.Update();
					if (componentExists)
					{
						ImGui::Text("");
						ImGui::Text("WARNING");
						ImGui::Text("This component is not really intended to be modified since is a");
						ImGui::Text("component that relies on external files and not normal values");
					}
					closeBtn.Update();
					ImGui::SameLine();
					addBtn.Update();
					popup.End();
				}
			}

			void Activate(bool componentExists)
			{
				this->componentExists = componentExists;
				if (componentExists)
				{
					AudioComponent* ac = SceneManager::GetScene(SceneManager::currentScene)->em.getComponent<AudioComponent>(selectedEntity);
					textBox.SetText(ac->name);
				}
				popup.Open();
			}

			void AddAudioComponent()
			{
				if (textBox.GetText() == "") return;
				bool canClose = false;
				Scene* currentScene = SceneManager::GetScene(SceneManager::currentScene);

				if (!componentExists)
				{
					if (currentScene->em.addComponent<AudioComponent>(this->selectedEntity, textBox.GetText()))
					{
						canClose = true;
					}
				}
				else
				{
					std::string oldName = "";

					{
						AudioComponent* ac = currentScene->em.getComponent<AudioComponent>(selectedEntity);
						oldName = ac->name;
					}

					if (currentScene->em.RemoveComponent<AudioComponent>(selectedEntity))
					{
						if (currentScene->em.addComponent<AudioComponent>(this->selectedEntity, textBox.GetText()))
						{
							canClose = true;
						}
						else
						{
							currentScene->em.addComponent<AudioComponent>(this->selectedEntity, oldName);
						}
					}
					
					
				}

				if (!canClose) return;
				popup.Close();
				if (this->updateComponents)
				{
					this->updateComponents();
				}
			}

		};
	}
}

