#pragma once
#include "../../../../API/APIExport.h"

namespace D2Maker
{
	namespace GUI
	{
		class VelocityCreator
		{
		private:
			Entity& selectedEntity;
			std::function<void()> closeCreation;
			
			GUIAPI::TextField textBox_dx;
			GUIAPI::TextField textBox_dy;
			GUIAPI::TextField textBox_dtheta;

			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;

			GUIAPI::PopUp popup;
		public:
			VelocityCreator(Entity& selectedEntity,std::function<void()> closeCreation) :
				selectedEntity(selectedEntity),
				closeCreation(closeCreation),
				textBox_dx("dx"),
				textBox_dy("dy"),
				textBox_dtheta("d-theta"),
				popup("Add - Velocity"),
				addBtn(100, 30, "Add", [this](void)
					{
						AddVelocityComponent();
					}
				),
				closeBtn(100, 30, "Close", [this](void)
					{
						popup.Close();
					}
				)
			{ }

			void Update()
			{
				if (popup.Begin())
				{
					textBox_dx.Update();
					textBox_dy.Update();
					textBox_dtheta.Update();
					closeBtn.Update();
					ImGui::SameLine();
					addBtn.Update();
					popup.End();
				}
			}

			void AddVelocityComponent()
			{
				float dx=0;
				float dy=0;
				float dtheta=0;
				if (!ConvertStringToNum<float>(textBox_dx.GetText(), dx))			return;
				if (!ConvertStringToNum<float>(textBox_dy.GetText(), dy))			return;
				if (!ConvertStringToNum<float>(textBox_dtheta.GetText(), dtheta))	return;

				if (SceneManager::GetScene(SceneManager::currentScene)->em.addComponent<Velocity>(
					selectedEntity, dx, dy, dtheta))
				{
					popup.Close();
					if (this->closeCreation)
					{
						this->closeCreation();
					}
				}
			}

			void Activate()
			{
				popup.Open();
			}


			



		};
	}
}
