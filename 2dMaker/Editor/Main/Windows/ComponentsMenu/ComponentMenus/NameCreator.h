#pragma once
//this will stay right here since it will be deprecated so no .cpp file
//however the functions are now set to inline 
#include "../../../../API/APIExport.h"
namespace D2Maker
{
	namespace GUI
	{
		class NameCreator
		{
		private:
			Entity& selectedEntity;
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			GUIAPI::TextField textBox;
			std::function<void()> updateComponents;
			bool componentExists = false;
		public:
			NameCreator(Entity& selectedEntity, std::function<void()> closeCreation) : updateComponents(updateComponents),
				popup("Name"),
				selectedEntity(selectedEntity),
				textBox("Name"),
				addBtn(100, 30, "Add/Mod", [this](void)
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

			void Activate(bool componentExists)
			{
				this->componentExists = componentExists;
				if (componentExists)
				{
					Name* name = SceneManager::GetScene(SceneManager::currentScene)->em.getComponent<Name>(selectedEntity);
					textBox.SetText(name->name);
				}
				popup.Open();
			}

			void AddNameComponent()
			{
				bool canClose = false;
				if (textBox.GetText() == "") return;

				if (componentExists)
				{
					if (SceneManager::GetScene(SceneManager::currentScene)->em.addComponent<Name>(this->selectedEntity, textBox.GetText()))
					{
						CONSOLELOGCOMPONENTSUCCESS(Name, this->selectedEntity);
						canClose = true;
					}
					else { CONSOLELOGCOMPONENTUNSUCCESFUL(Name, this->selectedEntity); }
				}
				else
				{
					Name* name = SceneManager::GetScene(SceneManager::currentScene)->em.getComponent<Name>(selectedEntity);
					name->name = textBox.GetText();
					canClose = true;
					CONSOLELOGCOMPONENTSUCCESSMODIFY(Name, this->selectedEntity);
				}




				if (!canClose) return;
				popup.Close();
				if (this->updateComponents)
				{
					this->updateComponents();
				}
			}

		};
	}
}

