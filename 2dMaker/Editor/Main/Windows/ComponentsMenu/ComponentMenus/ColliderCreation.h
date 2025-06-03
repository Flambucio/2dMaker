#pragma once
#include "../../../../API/APIExport.h"
namespace D2Maker
{
	namespace GUI
	{
		class ColliderCreator
		{
		private:
			Entity& selectedEntity;
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			std::function<void()> closeCreation;
		public:
			ColliderCreator(Entity& selectedEntity,std::function<void()> closeCreation) : closeCreation(closeCreation),
				popup("Collider  Menu"),
				selectedEntity(selectedEntity),
				addBtn(100, 30, "Add", [this](void)
					{
						if (SceneManager::GetScene(SceneManager::currentScene)->em.addComponent<Collider>(this->selectedEntity))
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
			{}
			
			void Update()
			{
				//TRACE("update popup");
				if (popup.Begin())
				{
					TRACE("true");
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
