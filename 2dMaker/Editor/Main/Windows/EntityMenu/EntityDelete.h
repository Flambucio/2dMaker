#pragma once
#include "../../../API/APIExport.h"

namespace D2Maker
{
	namespace GUI
	{
		class EntityDeletePopup
		{
		private:
			std::function<void()> deleteCallback;
			GUIAPI::ButtonWithCallback<> close;
			GUIAPI::ButtonWithCallback<> confirm;
			GUIAPI::PopUp popup;
		public:
			EntityDeletePopup(std::function<void()> deleteCallback) : popup("Delete"),deleteCallback(deleteCallback),
				close(100, 30, "Close", [this]()
					{
						popup.Close();
					}
				),
				confirm(100, 30, "Confirm", [this]()
					{
						if (this->deleteCallback)
						{
							this->deleteCallback();
							this->popup.Close();
						}
						
						
					}
				)
			{ }

			void Update()
			{
				if (popup.Begin())
				{
					ImGui::Text("Are you sure you want to delete this entity?");
					close.Update();
					ImGui::SameLine();
					confirm.Update();
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
