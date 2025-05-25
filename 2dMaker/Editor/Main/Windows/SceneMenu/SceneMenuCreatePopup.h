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
			SceneMenuCreatePopup(std::function<void()> lSNC) : popup("Create Scene Menu"), textName("Scene Name:"),
				createSceneBtn(150, 30, "Create", [this]()
					{
						if (SceneManager::AddScene(textName.GetText()))
						{
							if (loadSceneNamesCallback)
							{
								loadSceneNamesCallback();
							}
							popup.Close();
						}
					}
				),
				closeSceneBtn(150, 30, "Close", [this]()
					{
						popup.Close();
					}
				),
				loadSceneNamesCallback(lSNC)
			{ }

			void Update()
			{
				if (popup.Begin())
				{
					textName.Update();
					createSceneBtn.Update();
					ImGui::SameLine();
					closeSceneBtn.Update();
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