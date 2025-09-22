#pragma once
#include "../../Editor/API/APIExport.h"

namespace D2Maker
{
	namespace GUI
	{
		class Console
		{
		private:
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> closeBtn;
			GUIAPI::ButtonWithCallback<> clearBtn;
		public:
			Console() : popup("Console"),
				closeBtn(300, 30, "Close", [this](void)
					{
						popup.Close();
						ConsoleOpen::value = false;
					}),
				clearBtn(300, 30, "Clear", [this](void)
					{
						ConsoleToRender::toRender.clear();
						ConsoleToRender::count = 0;
						ConsoleToRender::Log((std::to_string(ConsoleToRender::count) + ". " + "Logs Cleared!"));
						ConsoleToRender::toRender.shrink_to_fit();
					})
			{}


			void Update()
			{
				OpenC();

				if (ConsoleOpen::value)
				{
					ImGui::SetNextWindowSize(ImVec2(620, 550));
					if (popup.Begin())
					{
						ImGui::BeginChild("ConsoleLogs", ImVec2(0, -40), 0, 0);
						for (int i = 0;i < ConsoleToRender::toRender.size();i++)
						{
							ImGui::Text(ConsoleToRender::toRender[i].c_str());
						}
						ImGui::EndChild();
						ImGui::Separator();

						clearBtn.Update();
						ImGui::SameLine();
						closeBtn.Update();
						popup.End();
					}
					else if (ConsoleOpen::value) { ConsoleOpen::value = false; }
					
				}
				
			}

			void OpenC()
			{
				if (ConsoleShouldOpen::value && !ConsoleOpen::value)
				{
					ConsoleOpen::value = true;
					popup.Open();
				}

				if (ConsoleShouldOpen::value) { ConsoleShouldOpen::value = false; }
				
			}

		};
	}

}


