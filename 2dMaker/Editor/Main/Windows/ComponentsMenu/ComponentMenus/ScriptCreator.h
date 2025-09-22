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
			std::function<void()> updateComponents;
			GUIAPI::TextField textBox;
			bool componentExists=false;
			
		public:
			ScriptCreator(Entity& selectedEntity, std::function<void()> updateComponents) : updateComponents(updateComponents),
				popup("Script"),
				selectedEntity(selectedEntity),
				textBox("Filepath"),
				addBtn(180, 30, "Add/Mod", [this](void)
					{
						AddScriptComponent();
					}
				),
				closeBtn(180, 30, "Close", [this](void)
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
					ImGui::Text("The file should be into Projects/{currentProject}/Scripts/");
					if (componentExists)
					{
						ImGui::Text("");
						ImGui::Text("WARNING");
						ImGui::Text("This component is not really intended to be modified since is a");
						ImGui::Text("component that relies on external files and not normal values");
					}
					
					
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
					Script* script = SceneManager::GetScene(SceneManager::currentScene)->em.getComponent<Script>(selectedEntity);
					std::string Ttex = ((fs::path)script->filepath).filename().string();
					SliceStringReference(Ttex, 0, (Ttex.size() - 5));
					textBox.SetText(Ttex);
				}
				popup.Open();
			}

			void AddScriptComponent()
			{
				bool canClose = false;
				if (textBox.GetText() == "") return;
				Scene* currentScene = SceneManager::GetScene(SceneManager::currentScene);
				std::string path = "Projects/" + FileSys::currentProject + "/Scripts/" + textBox.GetText() + ".txt";

				if (!componentExists)
				{
					if (currentScene->em.addComponent<Script>(this->selectedEntity, path))
					{
						canClose = true;
						CONSOLELOGCOMPONENTSUCCESS(Script, this->selectedEntity);
					}
					else { CONSOLELOGCOMPONENTUNSUCCESFUL(Script, this->selectedEntity); }
				}
				else
				{
					std::string oldPath = "";
					{
						Script* script = currentScene->em.getComponent<Script>(selectedEntity);
						oldPath = script->filepath;
					}

					if (currentScene->em.RemoveComponent<Script>(selectedEntity))
					{
						if (currentScene->em.addComponent<Script>(selectedEntity, path))
						{
							canClose = true;
							CONSOLELOGCOMPONENTSUCCESSMODIFY(Script, this->selectedEntity);
						}
						else
						{
							currentScene->em.addComponent<Script>(selectedEntity, oldPath);
						}
					}

					
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
