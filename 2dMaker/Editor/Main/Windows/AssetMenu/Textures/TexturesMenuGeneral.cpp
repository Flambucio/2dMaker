#include "TexturesMenu.h"
using namespace D2Maker::GUI;
TexturesMenu::TexturesMenu() : menu(textureNames, 0, 250, 20),
deleteBtn(80, 30, "Delete", [this](void)
	{
		if (textureNames.empty()) return;
		tDel.Activate();
	}
),
addBtn(80, 30, "Add", [this](void)
	{
		tAdd.Activate();
	}
),
previewButton(80, 30, "Preview", [this](void)
	{
		if (textureNames.empty()) return;
		if (previewWindow.windowOpen) return;
		previewWindow.Initialize(menu.GetCurrentValue());
	}
),
tAdd([this](void) {LoadTextureNames();}),
tDel([this](void) {DeleteCallback();})

{
}

void TexturesMenu::LoadTextureNames()
{
	textureNames.clear();
	for (auto& texture : TextureLoader::GetMap())
	{
		textureNames.push_back(texture.first);
	}
	menu.UpdateValues(textureNames);
}

void TexturesMenu::Update()
{
	GUIAPI::GUIWindow::CreateFixedWindow(1000, 120, 280, 420, "TextureMenu");
	menu.Update();
	GUIAPI::GUIWindow::EndWindow();
	GUIAPI::GUIWindow::CreateFixedWindow(1000, 540, 280, 60, "TextureMenuControls");
	addBtn.Update();
	ImGui::SameLine();
	previewButton.Update();
	ImGui::SameLine();
	deleteBtn.Update();
	tAdd.Update();
	tDel.Update();

	GUIAPI::GUIWindow::EndWindow();
	previewWindow.Update();
}

void TexturesMenu::DeleteCallback()
{
	std::string name = menu.GetCurrentValue();
	TextureLoader::RemoveTexture(name);
	CONSOLELOG("Deleted texture: " + name);
	LoadTextureNames();
}

TextureAdding::TextureAdding(std::function<void()> updateList) : updateList(updateList),
addBtn(160, 30, "Add", [this](void)
	{
		if (this->AddTexture())
		{
			if (this->updateList)
			{
				this->updateList();
			}
			popup.Close();
		}
	}
),
closeBtn(160, 30, "Close", [this](void)
	{
		popup.Close();
	}
),
textBox_name("Name"),
textBox_path("Path"),
popup("Load Texture")
{
}

void TextureAdding::Update()
{
	if (popup.Begin())
	{
		textBox_name.Update();
		textBox_path.Update();
		ImGui::Text("The directory starts at ");
		ImGui::Text("Projects/{currentProject}/Resources/Textures/");
		ImGui::Text("subdirectories are not supported (yet)");
		ImGui::Text("insert only name of the file without extension");
		ImGui::Text("file should be png");
		closeBtn.Update();
		ImGui::SameLine();
		addBtn.Update();
		popup.End();
	}
}

bool TextureAdding::AddTexture()
{
	std::string basePath = "Projects/" + FileSys::currentProject + "/Resources/Textures/";
	std::string path = basePath + textBox_path.GetText() + ".png";
	std::string name = textBox_name.GetText();
	if (!fs::exists(path) || !fs::is_regular_file(path))
	{
		return false;
	}
	if (TextureLoader::Exists(name)) return false;
	TextureLoader::LoadTexture(name, path);
	CONSOLELOG("Texture loaded at: " + path + " at: " + name);
	return true;
}

TextureDelete::TextureDelete(std::function<void()> deleteTexture) : deleteTexture(deleteTexture),
confirmBtn(143, 30, "Confirm", [this](void)
	{
		if (this->deleteTexture)
		{
			this->deleteTexture();
			popup.Close();
		}

	}
),
closeBtn(143, 30, "Close", [this](void)
	{
		popup.Close();
	}
),
popup("Delete Texture")
{
}

void TextureDelete::Update()
{
	if (popup.Begin())
	{
		ImGui::Text("textures are not meant to be deleted");
		ImGui::Text("removing textures already used by entities");
		ImGui::Text("can lead to crashes/undefined behaviour");
		closeBtn.Update();
		ImGui::SameLine();
		confirmBtn.Update();
		popup.End();
	}
}