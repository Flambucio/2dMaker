#pragma once
#include "../../../API/APIExport.h"

namespace D2Maker
{
	namespace GUI
	{
		class EntityCreatePopup
		{
		private:
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> close;
			GUIAPI::ButtonWithCallback<> create;
			GUIAPI::TextField textBox;
			GUIAPI::CheckBox checkbox;
		public:
			EntityCreatePopup() :
				popup("Create Entity"),
				close(100, 30, "Close", [this](void)
					{
						popup.Close();
					}
				),
				create(100, 30, "Create", [this](void)
					{

					}
				),
				textBox("Entity Name"),
				checkbox("Is Virtual?")

			{ }

			void Update()
			{
				if (popup.Begin())
				{
					textBox.Update();
					checkbox.Update();
					close.Update();
					ImGui::SameLine();
					create.Update();
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