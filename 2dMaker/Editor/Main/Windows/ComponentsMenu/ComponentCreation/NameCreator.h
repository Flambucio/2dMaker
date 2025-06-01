#pragma once
#include "../../../../API/APIExport.h"
namespace D2Maker
{
	namespace GUI
	{
		class TimerCreator
		{
		private:
			Entity& selectedEntity;
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			GUIAPI::TextField textBox;
			std::function<void()> closeCreation;
		public:
			TimerCreator(Entity& selectedEntity, std::function<void()> closeCreation) : closeCreation(closeCreation),
				popup("Add - Timer"),
				selectedEntity(selectedEntity),
				textBox("Name"),
				addBtn(100, 30, "Add", [this](void)
					{
						AddNameComponent();
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

			void AddNameComponent()
			{
				if (textBox.GetText() == "") return;


				if (SceneManager::GetScene(SceneManager::currentScene)->em.addComponent<Name>(this->selectedEntity,textBox.GetText()))
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

