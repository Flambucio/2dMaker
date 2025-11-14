#include "AssetMenu.h"
using namespace D2Maker::GUI;

void AssetMenu::Update()
{
	if (!loadedAssetsStart)
	{
		aMenu.LoadAudioNames();
		tMenu.LoadTextureNames();
		loadedAssetsStart = true;
	}
	GUIAPI::GUIWindow::CreateFixedWindow(1000, 60, 280, 60, "AssetMenuSelect");
	dropdown.Update();
	GUIAPI::GUIWindow::EndWindow();
	if (dropdown.GetCurrVal() == dropdownValues[0])
	{
		tMenu.Update();
	}
	else if (dropdown.GetCurrVal() == dropdownValues[1])
	{
		aMenu.Update();
	}

}