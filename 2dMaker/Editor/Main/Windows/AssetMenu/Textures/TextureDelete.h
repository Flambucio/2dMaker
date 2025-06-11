#pragma once
#include "../../../../API/APIExport.h"

namespace D2Maker
{
	namespace GUI
	{
		class TextureDelete
		{
		private:
			GUIAPI::ButtonWithCallback<> confirmBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			GUIAPI::PopUp popup;
			std::function<void()> deleteTexture;
			
		public:
			TextureDelete(std::function<void()> deleteTexture) : deleteTexture(deleteTexture),
				confirmBtn(143, 30, "Confirm", [this](void)
					{
						if (this->deleteTexture)
						{
							this->deleteTexture();
							popup.Close();
						}
						
					}
				),
				closeBtn(143, 30,"Close", [this](void)
					{
						popup.Close();
					}
				),
				popup("Delete Texture")
			{ }

			void Update()
			{
				if (popup.Begin())
				{
					ImGui::Text("textures are not meant to be deleted");
					ImGui::Text("removing textures already used by entities");
					ImGui::Text("can lead to crashes/undefined behaviour");
					closeBtn.Update();
					ImGui::SameLine();
					confirmBtn.Update();
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
