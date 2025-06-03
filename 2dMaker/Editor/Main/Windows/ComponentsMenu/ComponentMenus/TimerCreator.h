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
			std::function<void()> closeCreation;
		public:
			TimerCreator(Entity& selectedEntity, std::function<void()> closeCreation) : closeCreation(closeCreation),
				popup("Add - Timer"),
				selectedEntity(selectedEntity),
				addBtn(100, 30, "Add", [this](void)
					{
						if (SceneManager::GetScene(SceneManager::currentScene)->em.addComponent<Timer>(this->selectedEntity))
						{
							popup.Close();
							if (this->closeCreation)
							{
								this->closeCreation();
							}
						}
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

		};
	}
}
