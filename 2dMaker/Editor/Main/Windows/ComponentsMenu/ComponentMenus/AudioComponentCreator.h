#pragma once
#include "../../../../API/APIExport.h"
namespace D2Maker
{
	namespace GUI
	{
		class AudioComponentCreator
		{
		private:
			Entity& selectedEntity;
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			std::function<void()> closeCreation;
			GUIAPI::TextField textBox;
		public:
			AudioComponentCreator(Entity& selectedEntity, std::function<void()> closeCreation) : closeCreation(closeCreation),
				popup("Add - Audio"),
				selectedEntity(selectedEntity),
				textBox("Audio Name"),
				addBtn(100, 30, "Add", [this](void)
					{
						AddAudioComponent();
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

			void Activate()
			{
				popup.Open();
			}

			void AddAudioComponent()
			{
				if (textBox.GetText() == "") return;
				if (SceneManager::GetScene(SceneManager::currentScene)->em.addComponent<AudioComponent>(this->selectedEntity,textBox.GetText()))
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

