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
			std::function<void()> closeCreation;

		public:

			TransformCreator(Entity&selectedEntity,std::function<void()> closeCreation) : textBox_x("X coord"),textBox_y("Y coord"),
				textBox_width("Width"),textBox_height("Height"),textBox_rotation("Rotation (degrees)"),
				selectedEntity(selectedEntity),popup("Add - Transform"),
				closeBtn(100, 30, "Close", [this](void)
					{
						popup.Close();
					}
				),
				addBtn(100, 30, "Add", [this](void)
					{
						AddTransformComponent();
					}
				),
				closeCreation(closeCreation)
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

			void Activate()
			{
				popup.Open();
			}

			void AddTransformComponent()
			{
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
				if (SceneManager::GetScene(SceneManager::currentScene)->em.addComponent<Transform>(
					selectedEntity, x, y, width, height, rotationDeg))
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
