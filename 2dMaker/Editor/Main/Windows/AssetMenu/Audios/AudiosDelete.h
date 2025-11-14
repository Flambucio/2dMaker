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
			AudioDelete(std::function<void()> deleteAudio);
			void Update();

			inline void Activate()
			{
				popup.Open();
			}
		};
	}
}


