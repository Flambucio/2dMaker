#pragma once
#include "../../../API/APIExport.h"
namespace D2Maker
{
	namespace GUI
	{
		class ResoulutionMenu
		{
		private:
			const std::vector<std::string> dropdownOptions = { "16/9","1/1","9/16" };
			GUIAPI::Dropdown dropdown;
			
			GUIAPI::TextField textBox_Width;
			GUIAPI::TextField textBox_VirtualWidth;
			GUIAPI::ButtonWithCallback<> closeBtn;
			GUIAPI::ButtonWithCallback<> confirmBtn;
			GUIAPI::CheckBox fullscreenBox;
			GUIAPI::PopUp popup;
		public:
			ResoulutionMenu() :
				dropdown(dropdownOptions, 0, "Aspect Ratio"),
				popup("Resoulution"),
				textBox_Width("Default Window Width (min=144)"), 
				textBox_VirtualWidth("Default Virtual Width (min=144)"),
				closeBtn(100, 30, "Close", [this](void)
					{
						popup.Close();
					}
				),
				confirmBtn(100, 30, "Confirm", [this](void)
					{
						ModifyResolution();
					}
				),
				fullscreenBox("Default Fullscreen?")
			{ }

			void Update()
			{
				if (popup.Begin())
				{
					dropdown.Update();
					textBox_Width.Update();
					textBox_VirtualWidth.Update();
					fullscreenBox.Update();
					ImGui::Text("Height is calculated through");
					ImGui::Text("aspect ratio automatically");
					ImGui::Text("");
					ImGui::Text("Some things may require restart");
					ImGui::Text("of the engine");
					closeBtn.Update();
					ImGui::SameLine();
					confirmBtn.Update();
					popup.End();
				}
			}

			void ModifyResolution()
			{
				int defaultWidth = 0;
				int virtualWidth = 0;
				if (!ConvertStringToNum<int>(textBox_Width.GetText(), defaultWidth)) return;
				if (!ConvertStringToNum<int>(textBox_VirtualWidth.GetText(), virtualWidth)) return;
				GameOptions::aspectRatioStr = dropdown.GetCurrVal();
				if (GameOptions::aspectRatioStr == dropdownOptions[1])
				{
					GameOptions::aspectRatioNumerator = 1;
					GameOptions::aspectRatioDenominator = 1;
				}
				else if (GameOptions::aspectRatioStr == dropdownOptions[2])
				{
					GameOptions::aspectRatioNumerator = 9;
					GameOptions::aspectRatioDenominator = 16;
				}
				else
				{
					GameOptions::aspectRatioNumerator = 16;
					GameOptions::aspectRatioDenominator = 9;
				}

				if (defaultWidth >= 144)
				{
					GameOptions::defaultWidth = defaultWidth;
					GameOptions::defaultHeight = static_cast<int>(
						(static_cast<float>(defaultWidth) * GameOptions::aspectRatioDenominator) / 
						GameOptions::aspectRatioNumerator);
				}

				if (virtualWidth >= 144)
				{
					TRACE("update virtual width");
					GameOptions::virtualWidth = virtualWidth;
					GameOptions::virtualHeight = static_cast<int>(
						(static_cast<float>(virtualWidth) * GameOptions::aspectRatioDenominator) / 
						GameOptions::aspectRatioNumerator);
				}
				GameOptions::fullScreen = fullscreenBox.GetValue();
				CONSOLELOG("Modified resolution to: " + std::to_string(GameOptions::virtualWidth) +
					"x" + std::to_string(GameOptions::virtualHeight));
				CONSOLELOG("Modified default size to: " + std::to_string(GameOptions::defaultWidth) +
					"x" + std::to_string(GameOptions::defaultHeight));
				CONSOLELOG("Modified aspect ratio to: " + std::to_string(GameOptions::aspectRatioNumerator) +
					"/" + std::to_string(GameOptions::aspectRatioDenominator));
				std::string fullscreenValueStr = GameOptions::fullScreen ? "true" : "false";
				CONSOLELOG("Set default fullscreen to: " + fullscreenValueStr);
				popup.Close();

			}

			void Activate()
			{
				popup.Open();
				dropdown.SetCurrVal(GameOptions::aspectRatioStr);
				textBox_Width.SetText(std::to_string(GameOptions::defaultWidth));
				textBox_VirtualWidth.SetText(std::to_string(GameOptions::virtualWidth));
				fullscreenBox.SetValue(GameOptions::fullScreen);
			}
		}; 
	}
}
