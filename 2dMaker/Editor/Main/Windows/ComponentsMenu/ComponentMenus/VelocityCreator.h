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
			std::function<void()> updateComponents;
			
			GUIAPI::TextField textBox_dx;
			GUIAPI::TextField textBox_dy;
			GUIAPI::TextField textBox_dtheta;

			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;

			GUIAPI::PopUp popup;

			bool componentExists=false;
		public:
			VelocityCreator(Entity& selectedEntity,std::function<void()> updateComponents) :
				selectedEntity(selectedEntity),
				updateComponents(updateComponents),
				textBox_dx("dx"),
				textBox_dy("dy"),
				textBox_dtheta("d-theta"),
				popup("Velocity"),
				addBtn(100, 30, "Add/Mod", [this](void)
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
				bool canClose = false;
				float dx=0;
				float dy=0;
				float dtheta=0;
				if (!ConvertStringToNum<float>(textBox_dx.GetText(), dx))			return;
				if (!ConvertStringToNum<float>(textBox_dy.GetText(), dy))			return;
				if (!ConvertStringToNum<float>(textBox_dtheta.GetText(), dtheta))	return;

				
				if (!componentExists)
				{
					if (SceneManager::GetScene(SceneManager::currentScene)->em.addComponent<Velocity>(
						selectedEntity, dx, dy, dtheta))
					{
						CONSOLELOGCOMPONENTSUCCESS(Velocity, this->selectedEntity);
						canClose = true;
					}
					else { CONSOLELOGCOMPONENTUNSUCCESFUL(Velocity, this->selectedEntity); }
				}
				else
				{
					Velocity* vel = SceneManager::GetScene(SceneManager::currentScene)->em.getComponent<Velocity>(selectedEntity);
					vel->defaultDx = dx;
					vel->defaultDy = dy;
					vel->defaultDtheta = dtheta;
					canClose = true;
					CONSOLELOGCOMPONENTSUCCESSMODIFY(Velocity, this->selectedEntity);
				}

				if (!canClose) return;
				popup.Close();
				if (updateComponents)
				{
					updateComponents();
				}
			}

			void Activate(bool componentExists)
			{
				this->componentExists = componentExists;
				if (componentExists)
				{
					Velocity* vel = SceneManager::GetScene(SceneManager::currentScene)->em.getComponent<Velocity>(selectedEntity);
					textBox_dx.SetText(std::to_string(vel->defaultDx));
					textBox_dy.SetText(std::to_string(vel->defaultDy));
					textBox_dtheta.SetText(std::to_string(vel->defaultDtheta));
					
				}
				else
				{
					textBox_dx.SetText(std::to_string(0));
					textBox_dy.SetText(std::to_string(0));
					textBox_dtheta.SetText(std::to_string(0));
				}
				popup.Open();
			}


			



		};
	}
}
