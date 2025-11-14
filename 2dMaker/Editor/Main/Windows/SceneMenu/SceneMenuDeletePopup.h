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
			SceneMenuDeletePopup(std::function<void()> deleteCallback);
			void Update();
			inline void Activate()
			{
				popup.Open();
			}
		};
	}
}