#pragma once
#include "../../../../API/APIExport.h"
namespace D2Maker
{
	namespace GUI
	{
		class RigidbodyCreator
		{
		private:
			Entity& selectedEntity;
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			std::function<void()> updateComponents;
			GUIAPI::TextField textBox_Mass;
			GUIAPI::TextField textBox_BounceFactor;
			GUIAPI::TextField textBox_MaxDy;
			bool componentExists = false;
		public:
			RigidbodyCreator(Entity& selectedEntity, std::function<void()> closeCreation) : updateComponents(updateComponents),
				popup("Rigidbody"),
				selectedEntity(selectedEntity),
				textBox_Mass("Mass"),
				textBox_BounceFactor("Bounciness (factor)"),
				textBox_MaxDy("Max gravity speed (dy)"),
				addBtn(100, 30, "Add/Mod", [this](void)
					{
						AddRigidBodyComponent();
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
					textBox_Mass.Update();
					textBox_BounceFactor.Update();
					textBox_MaxDy.Update();
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
					RigidBody* rb = SceneManager::GetScene(SceneManager::currentScene)->em.getComponent<RigidBody>(selectedEntity);
					textBox_Mass.SetText(std::to_string(rb->mass));
					textBox_BounceFactor.SetText(std::to_string(rb->bounciness));
					textBox_MaxDy.SetText(std::to_string(rb->maxDy));
				}
				else
				{
					textBox_Mass.SetText(std::to_string(0));
					textBox_BounceFactor.SetText(std::to_string(0));
					textBox_MaxDy.SetText(std::to_string(0));
				}
				popup.Open();
			}

			void AddRigidBodyComponent()
			{
				float mass = 0;
				float bounciness = 0;
				float maxDy;
				bool canClose = false;
				if (!ConvertStringToNum<float>(textBox_Mass.GetText(), mass)) return;
				if (!ConvertStringToNum<float>(textBox_BounceFactor.GetText(), bounciness)) return;
				if (!ConvertStringToNum<float>(textBox_MaxDy.GetText(), maxDy)) return;
				Scene* currentScene = SceneManager::GetScene(SceneManager::currentScene);
				if (!componentExists)
				{
					if (currentScene->em.addComponent<RigidBody>(this->selectedEntity,
						mass, bounciness, maxDy))
					{
						canClose = true;
					}
				}
				else
				{
					RigidBody* rb = currentScene->em.getComponent<RigidBody>(selectedEntity);
					rb->bounciness = bounciness;
					rb->mass = mass;
					rb->maxDy = maxDy;
					canClose = true;
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

