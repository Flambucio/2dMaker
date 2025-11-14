#pragma once
#include "../../../API/APIExport.h"

namespace D2Maker
{
	namespace GUI
	{
		class SceneMenuCreatePopup
		{
		private:
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> createSceneBtn;
			GUIAPI::ButtonWithCallback<> closeSceneBtn;
			GUIAPI::TextField textName;
			std::function<void()> loadSceneNamesCallback;
		public:
			SceneMenuCreatePopup(std::function<void()> lSNC);
			void Update();
			inline void Activate()
			{
				popup.Open();
			}
		};
	}
}