#include "AudioMenu.h"
using namespace D2Maker::GUI;

AudioMenu::AudioMenu()
    : menu(audioNames, 0, 250, 20),
    deleteBtn(120, 30, "Delete", [this]()
        {
            if (audioNames.empty()) return;
            aDel.Activate();
        }),
    addBtn(120, 30, "Add", [this]()
        {
            aAdd.Activate();
        }),
    aDel([this]() { DeleteCallback(); }),
    aAdd([this]() { LoadAudioNames(); })
{
}

void AudioMenu::LoadAudioNames()
{
    audioNames.clear();
    for (auto& texture : AudioLoader::GetMap())
    {
        audioNames.push_back(texture.first);
    }
    menu.UpdateValues(audioNames);
}

void AudioMenu::Update()
{
    GUIAPI::GUIWindow::CreateFixedWindow(1000, 120, 280, 420, "TextureMenu");
    menu.Update();
    GUIAPI::GUIWindow::EndWindow();

    GUIAPI::GUIWindow::CreateFixedWindow(1000, 540, 280, 60, "TextureMenuControls");
    addBtn.Update();
    ImGui::SameLine();
    deleteBtn.Update();

    aAdd.Update();
    aDel.Update();

    GUIAPI::GUIWindow::EndWindow();
    previewWindow.Update();
}

void AudioMenu::DeleteCallback()
{
    std::string name = menu.GetCurrentValue();
    AudioLoader::RemoveAudio(name);
    CONSOLELOG("Deleted audio: " + name);
    LoadAudioNames();
}

AudioAdding::AudioAdding(std::function<void()> updateList) : updateList(updateList),
addBtn(160, 30, "Add", [this](void)
	{
		if (this->AddAudio())
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
popup("Load Audio")
{
}

void AudioAdding::Update()
{
	if (popup.Begin())
	{
		textBox_name.Update();
		textBox_path.Update();
		ImGui::Text("The directory starts at ");
		ImGui::Text("Projects/{currentProject}/Resources/Audios/");
		ImGui::Text("subdirectories are not supported (yet)");
		ImGui::Text("insert only name of the file without extension");
		ImGui::Text("file should be ogg");
		closeBtn.Update();
		ImGui::SameLine();
		addBtn.Update();
		popup.End();
	}
}

bool AudioAdding::AddAudio()
{
	std::string basePath = "Projects/" + FileSys::currentProject + "/Resources/Audios/";
	std::string path = basePath + textBox_path.GetText() + ".ogg";
	std::string name = textBox_name.GetText();
	if (!fs::exists(path) || !fs::is_regular_file(path))
	{
		return false;
	}
	if (AudioLoader::Exists(name)) return false;
	AudioLoader::LoadAudio(name, path);
	CONSOLELOG("Loaded audio at : " + path + " as: " + name);
	return true;
}

AudioDelete::AudioDelete(std::function<void()> deleteAudio) : deleteAudio(deleteAudio),
confirmBtn(143, 30, "Confirm", [this](void)
	{
		if (this->deleteAudio)
		{
			this->deleteAudio();
			popup.Close();
		}

	}
),
closeBtn(143, 30, "Close", [this](void)
	{
		popup.Close();
	}
),
popup("Delete Audio")
{
}

void AudioDelete::Update()
{
	if (popup.Begin())
	{
		ImGui::Text("audios are not meant to be deleted");
		ImGui::Text("removing audios already used by entities");
		ImGui::Text("can lead to crashes/undefined behaviour");
		closeBtn.Update();
		ImGui::SameLine();
		confirmBtn.Update();
		popup.End();
	}
}