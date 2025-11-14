#include "Options.h"
using namespace D2Maker::GUI;

GeneralOptions::GeneralOptions() :
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
{
}

void GeneralOptions::Update()
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

void GeneralOptions::ModifyOptions()
{
	if (textBox_Name.GetText() == "") return;
	GameOptions::gameName = textBox_Name.GetText();
	GameOptions::logoFile = textBox_Image.GetText();
	popup.Close();
}

void GeneralOptions::Activate()
{
	popup.Open();
	textBox_Image.SetText(GameOptions::logoFile);
	textBox_Name.SetText(GameOptions::gameName);
}


OptionsMenu::OptionsMenu() :
	resolutionBtn(585, 35, "Resolution", [this](void)
		{
			resMenu.Activate();
		}
	),
	generalBtn(585, 35, "General", [this](void)
		{
			generalOptions.Activate();
		}
	),
	saveBtn(585, 35, "Save", [this](void)
		{
			FileSys::Save();
			saveP.Activate();
			CONSOLELOG("Project saved!");
		}
	),
	consoleBtn(585, 35, "Console", [this](void)
		{
			ConsoleShouldOpen::value = true;
		}
	)
{
}

void OptionsMenu::Update()
{
	GUIAPI::GUIWindow::CreateFixedWindow(400, 410, 600, 190, "Options");
	ImGui::Text("Options");
	saveBtn.Update();
	generalBtn.Update();
	resolutionBtn.Update();
	consoleBtn.Update();
	saveP.Update();
	resMenu.Update();
	generalOptions.Update();
	GUIAPI::GUIWindow::EndWindow();
}


ResoulutionMenu::ResoulutionMenu() :
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
{
}

void ResoulutionMenu::Update()
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

void ResoulutionMenu::ModifyResolution()
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

void ResoulutionMenu::Activate()
{
	popup.Open();
	dropdown.SetCurrVal(GameOptions::aspectRatioStr);
	textBox_Width.SetText(std::to_string(GameOptions::defaultWidth));
	textBox_VirtualWidth.SetText(std::to_string(GameOptions::virtualWidth));
	fullscreenBox.SetValue(GameOptions::fullScreen);
}