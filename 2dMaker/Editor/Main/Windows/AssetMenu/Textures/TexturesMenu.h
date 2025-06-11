#pragma once

#include "TextureAdd.h"
#include "TextureDelete.h"

namespace D2Maker
{
	namespace GUI
	{
		class TexturesMenu
		{
		private:
			std::vector<std::string> textureNames = {};
			GUIAPI::Menu menu;
			GUIAPI::ButtonWithCallback<> deleteBtn;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> previewButton;
			TextureAdding tAdd;
			TextureDelete tDel;
			
		public:
			D2Maker::PreviewTextureWindow previewWindow;
			TexturesMenu() : menu(textureNames,0,250,20),
				deleteBtn(80, 30, "Delete", [this](void)
					{
						if (textureNames.empty()) return;
						tDel.Activate();
					}
				),
				addBtn(80, 30, "Add", [this](void)
					{
						tAdd.Activate();
					}
				),
				previewButton(80, 30, "Preview", [this](void)
					{
						if (textureNames.empty()) return;
						if (previewWindow.windowOpen) return;
						previewWindow.Initialize(menu.GetCurrentValue());
					}
				),
				tAdd([this](void) {LoadTextureNames();}),
				tDel([this](void) {DeleteCallback();})
			
			{}

			void LoadTextureNames()
			{
				textureNames.clear();
				for (auto& texture : TextureLoader::GetMap())
				{
					textureNames.push_back(texture.first);
				}
				menu.UpdateValues(textureNames);
			}

			void Update()
			{
				GUIAPI::GUIWindow::CreateFixedWindow(1000, 120, 280, 420, "TextureMenu");
				menu.Update();
				GUIAPI::GUIWindow::EndWindow();
				GUIAPI::GUIWindow::CreateFixedWindow(1000, 540, 280, 60, "TextureMenuControls");
				addBtn.Update();
				ImGui::SameLine();
				previewButton.Update();
				ImGui::SameLine();
				deleteBtn.Update();
				tAdd.Update();
				tDel.Update();
				
				GUIAPI::GUIWindow::EndWindow();
				previewWindow.Update();
			}

			void DeleteCallback()
			{
				std::string name = menu.GetCurrentValue();
				TextureLoader::RemoveTexture(name);
				LoadTextureNames();
			}
			
		};
	}
}
