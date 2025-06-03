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
		public:
			FollowCreator(Entity& selectedEntity, std::function<void()> closeCreation) : closeCreation(closeCreation),
				popup("Add - Follow"),
				selectedEntity(selectedEntity),
				textBox_NameToFollow("name to follow"),
				textBox_Vel("Velocity"),
				addBtn(100, 30, "Add", [this](void)
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

			void Activate()
			{
				popup.Open();
			}

			void AddFollowComponent()
			{
				float vel = 0;
				std::string name = textBox_NameToFollow.GetText();
				if (name == "") return;
				if (!ConvertStringToNum<float>(textBox_Vel.GetText(), vel)) return;

				if (SceneManager::GetScene(SceneManager::currentScene)->em.addComponent<Follow>(this->selectedEntity, vel,name))
				{
					popup.Close();
					if (this->closeCreation)
					{
						this->closeCreation();
					}
				}
				
			}

		};
	}
}
