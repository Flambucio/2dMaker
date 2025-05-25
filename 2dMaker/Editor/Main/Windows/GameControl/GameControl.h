#pragma once
#include "../../../API/APIExport.h"

namespace D2Maker
{
	namespace GUI
	{
		class GameControls
		{
		private:
			GUIAPI::ButtonWithCallback<> runBtn;
			GUIAPI::ButtonWithCallback<> stopBtn;
			GUIAPI::ButtonWithCallback<> pauseBtn;
		public:
			GameControls() : 
				runBtn(100, 30, "Run", [this](void)
					{

					}
				),
				stopBtn(100, 30, "Stop", [this](void)
					{

					}
				),
				pauseBtn(100, 30, "Pause", [this](void)
					{

					}
				)
			{ }

		};
	}
}
