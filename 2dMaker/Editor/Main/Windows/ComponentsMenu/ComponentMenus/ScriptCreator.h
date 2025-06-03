#pragma once
#include "../../../../API/APIExport.h"
namespace D2Maker
{
	namespace GUI
	{
		class ScriptCreator
		{
		private:
			Entity& selectedEntity;
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			std::function<void()> closeCreation;
			GUIAPI::TextField textBox;
			
		public:
			ScriptCreator(Entity& selectedEntity, std::function<void()> closeCreation) : closeCreation(closeCreation),
				popup("Add - Script"),
				selectedEntity(selectedEntity),
				textBox("Filename"),
				addBtn(153, 30, "Add", [this](void)
					{
						AddScriptComponent();
					}
				),
				closeBtn(153, 30, "Close", [this](void)
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
					ImGui::Text("Insert only the name of the file (no extension)");
					ImGui::Text("Custom subdirectories are not supported (yet)");
					ImGui::Text("file format: txt");
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

			void AddScriptComponent()
			{
				if (textBox.GetText() == "") return;
				std::string path = "Projects/" + FileSys::currentProject + "/Scripts/" + textBox.GetText() + ".txt";
				if (SceneManager::GetScene(SceneManager::currentScene)->em.addComponent<Script>(this->selectedEntity, path))
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
