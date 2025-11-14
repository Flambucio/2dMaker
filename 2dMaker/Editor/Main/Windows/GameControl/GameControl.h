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
			bool& runGameFlag;
		public:
			inline GameControls(bool& runGameFlag) : runGameFlag(runGameFlag),
				runBtn(260, 30, "Run", [this](void)
					{
						this->runGameFlag = true;
					}
				),
				stopBtn(260, 30, "Stop", [this](void)
					{
						SceneManager::GetScene(SceneManager::currentScene)->StopScene();
						this->runGameFlag = false;
					}
				),
				pauseBtn(260, 30, "Pause", [this](void)
					{
						this->runGameFlag = false;
					}
				)
			{ }


			inline void Update()
			{
				GUIAPI::GUIWindow::CreateFixedWindow(1000, 600, 280, 120, "GameControls");
				runBtn.Update();
				pauseBtn.Update();
				stopBtn.Update();
				GUIAPI::GUIWindow::EndWindow();
			}
			

		};
	}
}
