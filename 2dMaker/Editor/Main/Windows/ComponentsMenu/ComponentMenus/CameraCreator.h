#pragma once
#include "../../../../API/APIExport.h"
namespace D2Maker
{
	namespace GUI
	{
		class CameraCreator
		{
		private:
			Entity& selectedEntity;
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			std::function<void()> closeCreation;
			GUIAPI::TextField textBox_x;
			GUIAPI::TextField textBox_y;
			GUIAPI::CheckBox checkBox_enableX;
			GUIAPI::CheckBox checkBox_enableY;
			
		public:
			CameraCreator(Entity& selectedEntity, std::function<void()> closeCreation) : closeCreation(closeCreation),
				popup("Add - Camera"),
				textBox_x("Focus x"),
				textBox_y("Focus y"),
				checkBox_enableX("Enable X"),
				checkBox_enableY("Enable Y"),
				selectedEntity(selectedEntity),
				addBtn(100, 30, "Add", [this](void)
					{
						AddCameraComponent();
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
					textBox_x.Update();
					textBox_y.Update();
					checkBox_enableX.Update();
					checkBox_enableY.Update();
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

			void AddCameraComponent()
			{
				bool enableX = checkBox_enableX.GetValue();
				bool enableY = checkBox_enableY.GetValue();
				float x = 0;
				float y = 0;
				if (!ConvertStringToNum<float>(textBox_x.GetText(), x)) return;
				if (!ConvertStringToNum<float>(textBox_y.GetText(), y)) return;

				if (SceneManager::GetScene(SceneManager::currentScene)->em.addComponent<Camera>(this->selectedEntity,
					enableX,enableY,x,y))
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

