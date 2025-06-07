#pragma once
#include "../../../../API/APIExport.h"
namespace D2Maker
{
	namespace GUI
	{
		class FollowCreator
		{
		private:
			Entity& selectedEntity;
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			std::function<void()> closeCreation;
			GUIAPI::TextField textBox_NameToFollow;
			GUIAPI::TextField textBox_Vel;
			bool componentExists = false;
		public:
			FollowCreator(Entity& selectedEntity, std::function<void()> closeCreation) : closeCreation(closeCreation),
				popup("Follow"),
				selectedEntity(selectedEntity),
				textBox_NameToFollow("name to follow"),
				textBox_Vel("Velocity"),
				addBtn(100, 30, "Add/Mod", [this](void)
					{

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
					textBox_NameToFollow.Update();
					textBox_Vel.Update();
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
					Follow* follow = SceneManager::GetScene(SceneManager::currentScene)->em.getComponent<Follow>(selectedEntity);
					textBox_NameToFollow.SetText(follow->entityToFollow);
					textBox_Vel.SetText(std::to_string(follow->velocity));
				}
				else
				{
					textBox_Vel.SetText(std::to_string(0));
				}
				popup.Open();
			}

			void AddFollowComponent()
			{
				float vel = 0;
				std::string name = textBox_NameToFollow.GetText();
				bool canClose = false;
				if (name == "") return;
				if (!ConvertStringToNum<float>(textBox_Vel.GetText(), vel)) return;
				Scene* currentScene = SceneManager::GetScene(SceneManager::currentScene);

				if (!componentExists)
				{
					if (currentScene->em.addComponent<Follow>(this->selectedEntity, vel, name))
					{
						canClose = true;
					}
				}
				else
				{
					float oldVel = 0;
					std::string oldName = "";
					{
						Follow* follow = currentScene->em.getComponent<Follow>(selectedEntity);
						oldVel = follow->velocity;
						oldName = follow->entityToFollow;
						
					}
					if (currentScene->em.addComponent<Follow>(selectedEntity,vel, name))
					{
						canClose = true;
					}
					else
					{
						currentScene->em.addComponent<Follow>(selectedEntity, oldVel, oldName);
					}
				}

				if (!canClose) return;
				popup.Close();
				if (this->closeCreation)
				{
					this->closeCreation();
				}
				
			}

		};
	}
}
