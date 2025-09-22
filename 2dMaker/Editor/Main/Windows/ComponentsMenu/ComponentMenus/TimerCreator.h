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
			std::function<void()> updateComponents;
		public:
			TimerCreator(Entity& selectedEntity, std::function<void()> closeCreation) : updateComponents(updateComponents),
				popup("Timer"),
				selectedEntity(selectedEntity),
				addBtn(100, 30, "Add", [this](void)
					{
						if (SceneManager::GetScene(SceneManager::currentScene)->em.addComponent<Timer>(this->selectedEntity))
						{
							CONSOLELOGCOMPONENTSUCCESS(Timer, this->selectedEntity);
							popup.Close();
							if (this->updateComponents)
							{
								this->updateComponents();
							}
						}
						else { CONSOLELOGCOMPONENTUNSUCCESFUL(Timer, this->selectedEntity); }
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

			void Activate(bool componentExists)
			{
				if (componentExists) return;
				popup.Open();
			}

		};
	}
}
