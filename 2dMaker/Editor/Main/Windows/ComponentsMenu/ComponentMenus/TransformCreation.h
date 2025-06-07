#pragma once
#include "../../../../API/APIExport.h"

namespace D2Maker
{
	namespace GUI
	{
		class TransformCreator
		{
		private:
			Entity& selectedEntity;
			GUIAPI::TextField textBox_x;
			GUIAPI::TextField textBox_y;
			GUIAPI::TextField textBox_width;
			GUIAPI::TextField textBox_height;
			GUIAPI::TextField textBox_rotation;
			GUIAPI::ButtonWithCallback<> closeBtn;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::PopUp popup;
			std::function<void()> updateComponents;
			bool componentExists = false;

		public:

			TransformCreator(Entity&selectedEntity,std::function<void()> updateComponents) : textBox_x("X coord"),textBox_y("Y coord"),
				textBox_width("Width"),textBox_height("Height"),textBox_rotation("Rotation (degrees)"),
				selectedEntity(selectedEntity),popup("Transform"),
				closeBtn(100, 30, "Close", [this](void)
					{
						popup.Close();
					}
				),
				addBtn(100, 30, "Add/Mod", [this](void)
					{
						AddTransformComponent();
					}
				),
				updateComponents(updateComponents)
			{

			}
			
			void Update()
			{
				if (popup.Begin())
				{
					textBox_x.Update();
					textBox_y.Update();
					textBox_width.Update();
					textBox_height.Update();
					textBox_rotation.Update();
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
					Scene* currentScene = SceneManager::GetScene(SceneManager::currentScene);
					Transform* transform = currentScene->em.getComponent<Transform>(selectedEntity);
					textBox_x.SetText(std::to_string(transform->defaultX));
					textBox_y.SetText(std::to_string(transform->defaultY));
					textBox_width.SetText(std::to_string(transform->width));
					textBox_height.SetText(std::to_string(transform->height));
					textBox_rotation.SetText(std::to_string(transform->rotationDegrees));

				}
				else
				{
					textBox_x.SetText(std::to_string(0));
					textBox_y.SetText(std::to_string(0));
					textBox_width.SetText(std::to_string(0));
					textBox_height.SetText(std::to_string(0));
					textBox_rotation.SetText(std::to_string(0));
				}
				popup.Open();
			}

			void AddTransformComponent()
			{
				bool canClose = false;
				float x=0;
				float y=0;
				float width=0;
				float height=0;
				float rotationDeg = 0;
				if(!ConvertStringToNum<float>(textBox_x.GetText(),x))					 return;
				if (!ConvertStringToNum<float>(textBox_y.GetText(), y))				 	 return;
				if (!ConvertStringToNum<float>(textBox_width.GetText(), width))			 return;
				if (!ConvertStringToNum<float>(textBox_height.GetText(), height))		 return;
				if (!ConvertStringToNum<float>(textBox_rotation.GetText(), rotationDeg)) return;
				if (!componentExists)
				{


					if (SceneManager::GetScene(SceneManager::currentScene)->em.addComponent<Transform>(
						selectedEntity, x, y, width, height, rotationDeg))
					{
						canClose = true;
					}
					
				}
				else
				{
					Transform* transform = SceneManager::GetScene(SceneManager::currentScene)->em.getComponent<Transform>(selectedEntity);
					transform->defaultX = x;
					transform->defaultY = y;
					transform->width = width;
					transform->height = height;
					transform->rotationDegrees = rotationDeg;
					canClose = true;

				}

				if (!canClose) return;
				popup.Close();
				if (updateComponents)
				{
					updateComponents();
				}

				
			}


		};
	}
}
