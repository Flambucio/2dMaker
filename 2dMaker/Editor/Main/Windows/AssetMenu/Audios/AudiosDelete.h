#pragma once
#include "../../../../API/APIExport.h"

namespace D2Maker
{
	namespace GUI
	{
		class AudioDelete
		{
		private:
			GUIAPI::ButtonWithCallback<> confirmBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			GUIAPI::PopUp popup;
			std::function<void()> deleteAudio;

		public:
			AudioDelete(std::function<void()> deleteAudio) : deleteAudio(deleteAudio),
				confirmBtn(143, 30, "Confirm", [this](void)
					{
						if (this->deleteAudio)
						{
							this->deleteAudio();
							popup.Close();
						}

					}
				),
				closeBtn(143, 30, "Close", [this](void)
					{
						popup.Close();
					}
				),
				popup("Delete Audio")
			{
			}

			void Update()
			{
				if (popup.Begin())
				{
					ImGui::Text("audios are not meant to be deleted");
					ImGui::Text("removing audios already used by entities");
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


