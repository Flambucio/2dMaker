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
			std::function<void()> closeCreation;
			GUIAPI::TextField textBox_Mass;
			GUIAPI::TextField textBox_BounceFactor;
			GUIAPI::TextField textBox_MaxDy;
		public:
			RigidbodyCreator(Entity& selectedEntity, std::function<void()> closeCreation) : closeCreation(closeCreation),
				popup("Add - Rigidbody"),
				selectedEntity(selectedEntity),
				textBox_Mass("Mass"),
				textBox_BounceFactor("Bounciness (factor)"),
				textBox_MaxDy("Max gravity speed (dy)"),
				addBtn(100, 30, "Add", [this](void)
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

			void Activate()
			{
				popup.Open();
			}

			void AddRigidBodyComponent()
			{
				float mass = 0;
				float bounciness = 0;
				float maxDy;
				if (!ConvertStringToNum<float>(textBox_Mass.GetText(), mass)) return;
				if (!ConvertStringToNum<float>(textBox_BounceFactor.GetText(), bounciness)) return;
				if (!ConvertStringToNum<float>(textBox_MaxDy.GetText(), maxDy)) return;

				if (SceneManager::GetScene(SceneManager::currentScene)->em.addComponent<RigidBody>(this->selectedEntity,
					mass,bounciness,maxDy))
				{
					TRACE("added");
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

