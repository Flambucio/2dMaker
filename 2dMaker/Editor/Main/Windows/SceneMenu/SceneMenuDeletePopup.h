#pragma once
#include "../../../API/APIExport.h"

namespace D2Maker
{
	namespace GUI
	{
		class SceneMenuDeletePopup
		{
		private:
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> close;
			GUIAPI::ButtonWithCallback<> confirm;
			std::function<void()> deleteCallback;
			
		public:
			SceneMenuDeletePopup(std::function<void()> deleteCallback) : 
				popup("Delete"),deleteCallback(deleteCallback),
				close(130, 30, "Close", [this](void)
					{
						popup.Close();
					}
				),
				confirm(130, 30, "Confirm", [this](void)
					{
						if (this->deleteCallback)
						{
							this->deleteCallback();
						}
						popup.Close();
					}
				)
			{ }

			void Update()
			{
				if (popup.Begin())
				{
					ImGui::Text("Are you sure you want to delete this scene?");
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