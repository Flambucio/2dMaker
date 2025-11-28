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
			std::function<void()> updateCallback;
			GUIAPI::ButtonWithCallback<> close;
			GUIAPI::ButtonWithCallback<> confirm;
			GUIAPI::PopUp popup;
		public:
			EntityDeletePopup(std::function<void()> deleteCallback,std::function<void()> updateCallback);
			void Update();

			inline void Activate()
			{
				popup.Open();
			}

		};
	}
}
