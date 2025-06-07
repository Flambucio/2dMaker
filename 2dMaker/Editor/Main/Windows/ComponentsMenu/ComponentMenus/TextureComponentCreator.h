#pragma once
#include "../../../../API/APIExport.h"
namespace D2Maker
{
	namespace GUI
	{
		class TextureComponentCreator
		{
		private:
			Entity& selectedEntity;
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			std::function<void()> updateComponents;
			GUIAPI::TextField textBox_Name;
			GUIAPI::TextField textBox_OIL; // order in layer
			bool componentExists = false;
		public:
			TextureComponentCreator(Entity& selectedEntity, std::function<void()> updateComponents) : updateComponents(updateComponents),
				popup("Texture"),
				selectedEntity(selectedEntity),
				textBox_Name("Texture Name"),
				textBox_OIL("Order in Layer"),
				addBtn(100, 30, "Add/Mod", [this](void)
					{
						AddTextureComponent();
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
					textBox_Name.GetText();
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
					TextureComponent* tc = SceneManager::GetScene(SceneManager::currentScene)->em.getComponent<TextureComponent>(selectedEntity);
					textBox_Name.SetText(tc->name);
					textBox_OIL.SetText(std::to_string(tc->orderInLayer));
				}
				else
				{
					textBox_OIL.SetText(std::to_string(0));
				}
				popup.Open();
			}

			void AddTextureComponent()
			{
				int oil = 0; //order in layer
				if (textBox_Name.GetText() == "") return;
				if (!ConvertStringToNum<int>(textBox_OIL.GetText(), oil)) return;
				bool canClose = false;
				Scene* currentScene = SceneManager::GetScene(SceneManager::currentScene);

				if (!componentExists)
				{
					if (currentScene->em.addComponent<TextureComponent>(this->selectedEntity,
						textBox_Name.GetText(), oil))
					{
						canClose = true;
					}
				}
				else
				{
					int oldOil = 0;
					std::string oldTextName = "";
					{
						TextureComponent* textureComponent = currentScene->em.getComponent<TextureComponent>(selectedEntity);
						oldOil = textureComponent->orderInLayer;
						oldTextName = textureComponent->name;
					}
					if (currentScene->em.RemoveComponent<TextureComponent>(selectedEntity))
					{
						if (currentScene->em.addComponent<TextureComponent>(selectedEntity,textBox_Name.GetText(), oil))
						{
							canClose = true;
						}
						else
						{
							currentScene->em.addComponent<TextureComponent>(selectedEntity,oldTextName, oldOil);
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


