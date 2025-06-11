#pragma once
#include "AudiosAdd.h"
#include "AudiosDelete.h"

namespace D2Maker
{
	namespace GUI
	{
		class AudioMenu
		{
		private:
			std::vector<std::string> audioNames = {};
			GUIAPI::Menu menu;
			GUIAPI::ButtonWithCallback<> deleteBtn;
			GUIAPI::ButtonWithCallback<> addBtn;
			AudioAdding aAdd;
			AudioDelete aDel;

		public:
			D2Maker::PreviewTextureWindow previewWindow;
			AudioMenu() : menu(audioNames, 0, 250, 20),
				deleteBtn(80, 30, "Delete", [this](void)
					{
						if (audioNames.empty()) return;
						aDel.Activate();
					}
				),
				addBtn(80, 30, "Add", [this](void)
					{
						aAdd.Activate();
					}
				),
				aDel([this](void) {DeleteCallback();}),
				aAdd([this](void) {LoadAudioNames();})

			{
			}

			void LoadAudioNames()
			{
				audioNames.clear();
				for (auto& texture : AudioLoader::GetMap())
				{
					audioNames.push_back(texture.first);
				}
				menu.UpdateValues(audioNames);
			}

			void Update()
			{
				GUIAPI::GUIWindow::CreateFixedWindow(1000, 120, 280, 420, "TextureMenu");
				menu.Update();
				GUIAPI::GUIWindow::EndWindow();
				GUIAPI::GUIWindow::CreateFixedWindow(1000, 540, 280, 60, "TextureMenuControls");
				addBtn.Update();
				ImGui::SameLine();
				ImGui::SameLine();
				deleteBtn.Update();
				aAdd.Update();
				aDel.Update();

				GUIAPI::GUIWindow::EndWindow();
				previewWindow.Update();
			}

			void DeleteCallback()
			{
				std::string name = menu.GetCurrentValue();
				AudioLoader::RemoveAudio(name);
				LoadAudioNames();
			}

		};
	}
}
