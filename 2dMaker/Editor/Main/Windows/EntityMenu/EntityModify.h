#pragma once
#include "../../../API/APIExport.h"

namespace D2Maker
{
	namespace GUI
	{
		class EntityModifyPopup
		{
		private:
			GUIAPI::TextField textBox;
			GUIAPI::ButtonWithCallback<> closeBtn;
			GUIAPI::ButtonWithCallback<> confirmBtn;
			GUIAPI::PopUp popup;
			std::string currentEntityName = "";
		public:
			EntityModifyPopup() : textBox("New Name"), popup("Modify Entity"),
				closeBtn(100, 30, "Close", [this](void)
					{
						popup.Close();
					}
				),
				confirmBtn(100, 30, "Confirm", [this](void)
					{
						if (this->textBox.GetText() == "") return;
						if(SceneManager::GetScene(SceneManager::currentScene)->
							em.renameEntity(currentEntityName, this->textBox.GetText()))
						{
							popup.Close();
						}
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
					confirmBtn.Update();
					popup.End();
				}
			}

			void Activate(std::string currentEntityName)
			{
				this->currentEntityName = currentEntityName;
				popup.Open();
			}

		};
	}
}
