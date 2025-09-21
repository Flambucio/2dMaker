#pragma once

#include "../../../API/APIExport.h"

namespace D2Maker
{
	namespace GUI
	{
		class SavePopup
		{
		private:
			GUIAPI::ButtonWithCallback<> closeBtn;
			GUIAPI::PopUp popup;

		public:
			SavePopup() :
				closeBtn(100, 30, "Close", [this](void)
					{
						popup.Close();
					}
				),
				popup("Save") {
			}

			void Update()
			{
				if (popup.Begin())
				{
					ImGui::Text("Project saved!");
					closeBtn.Update();
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
