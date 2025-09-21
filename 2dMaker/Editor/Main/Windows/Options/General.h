#pragma once
#include "../../../API/APIExport.h"


namespace D2Maker
{
	namespace GUI
	{
		class GeneralOptions
		{
		private:
			GUIAPI::ButtonWithCallback<> confirmBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			GUIAPI::PopUp popup;
			GUIAPI::TextField textBox_Image;
			GUIAPI::TextField textBox_Name;

		public:
			GeneralOptions() :
				closeBtn(100, 30, "Close", [this](void)
					{
						popup.Close();
					}
				),
				confirmBtn(100, 30, "Confirm", [this](void)
					{
						ModifyOptions();
					}
				),
				popup("General"),
				textBox_Image("Icon name (no extension)"),
				textBox_Name("Game name")
			{}

			void Update()
			{
				if (popup.Begin())
				{
					textBox_Name.Update();
					textBox_Image.Update();
					ImGui::Text("Path for icon file");
					ImGui::Text("Projects/{currentProject}/");
					ImGui::Text("");
					ImGui::Text("Some things may require restart");
					ImGui::Text("of the engine");
					closeBtn.Update();
					ImGui::SameLine();
					confirmBtn.Update();
					popup.End();
				}
					
			}

			void ModifyOptions()
			{
				if (textBox_Name.GetText() == "") return;
				GameOptions::gameName = textBox_Name.GetText();
				GameOptions::logoFile = textBox_Image.GetText();
				popup.Close();
			}

			void Activate()
			{
				popup.Open();
				textBox_Image.SetText(GameOptions::logoFile);
				textBox_Name.SetText(GameOptions::gameName);
			}
		};
	}
}