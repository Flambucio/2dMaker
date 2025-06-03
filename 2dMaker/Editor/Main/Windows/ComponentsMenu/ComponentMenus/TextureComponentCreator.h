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
			std::function<void()> closeCreation;
			GUIAPI::TextField textBox_Name;
			GUIAPI::TextField textBox_OIL; // order in layer
		public:
			TextureComponentCreator(Entity& selectedEntity, std::function<void()> closeCreation) : closeCreation(closeCreation),
				popup("Add - Texture"),
				selectedEntity(selectedEntity),
				textBox_Name("Texture Name"),
				textBox_OIL("Order in Layer"),
				addBtn(100, 30, "Add", [this](void)
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

			void AddTextureComponent()
			{
				int oil = 0; //order in layer
				if (textBox_Name.GetText() == "") return;
				if (!ConvertStringToNum<int>(textBox_OIL.GetText(), oil)) return;
				if (SceneManager::GetScene(SceneManager::currentScene)->em.addComponent<TextureComponent>(this->selectedEntity, 
					textBox_Name.GetText(),oil))
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


