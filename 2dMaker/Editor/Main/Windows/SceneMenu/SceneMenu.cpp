#include "SceneMenu.h"
using namespace D2Maker::GUI;


SceneMenu::SceneMenu(GLFWwindow*& window) : window(window), scenesDropdown({}, 0, "Current Scene"),
noneDropdown({ "(None)" }, 0, "Current Scene"), SMDPopup([this](void) {this->DeleteFunc();}),
createScene(130, 20, "Create Scene", [this](void)
	{
		SMCPopup.Activate();
	}
),

deleteBtn(130, 20, "Delete Scene", [this](void)
	{
		SMDPopup.Activate();
	}
),
setDefault(150, 20, "Set Default Scene", [this](void)
	{
		SceneManager::defaultScene = SceneManager::currentScene;
		CONSOLELOG("Set default scene as: " + SceneManager::defaultScene);
	}
),
SMCPopup([this](void)
	{
		LoadSceneNames();
	}
)

{

}

void SceneMenu::Update()
{
	GUIAPI::GUIWindow::CreateFixedWindow(0, 0, 1280, 60, "sceneWindow");

	if (sceneNames.empty())
	{
		noneDropdown.Update();
	}
	else
	{
		scenesDropdown.Update();
	}

	ImGui::SameLine();
	createScene.Update();
	ImGui::SameLine();
	deleteBtn.Update();
	ImGui::SameLine();
	setDefault.Update();
	SMCPopup.Update();
	SMDPopup.Update();
	GUIAPI::GUIWindow::EndWindow();
	std::string sceneName = scenesDropdown.GetCurrVal();
	if (sceneName != SceneManager::currentScene)
	{
		SceneManager::SelectScene(sceneName);

	}
}

void SceneMenu::LoadSceneNames()
{
	sceneNames.clear();
	for (auto& element : SceneManager::scenes)
	{
		sceneNames.push_back(element.first);

	}
	scenesDropdown.UpdateValues(sceneNames);
}

void SceneMenu::DeleteFunc()
{
	std::string toRemove = SceneManager::currentScene;
	SceneManager::RemoveScene(toRemove);
	CONSOLELOG("Deleted scene: " + toRemove);
	if (!sceneNames.empty())
	{
		LoadSceneNames();
	}
	else
	{
		sceneNames = {};
	}

	std::string nextScene;
	!sceneNames.empty() ? nextScene = sceneNames[0] : nextScene = "";
	SceneManager::SelectScene(nextScene);
	scenesDropdown.SetCurrVal(nextScene);
	if (SceneManager::defaultScene == toRemove) SceneManager::defaultScene = nextScene;
}

SceneMenuCreatePopup::SceneMenuCreatePopup(std::function<void()> lSNC) : popup("Create Scene Menu"), textName("Scene Name:"),
createSceneBtn(150, 30, "Create", [this]()
	{
		if (SceneManager::AddScene(textName.GetText()))
		{
			CONSOLELOG("Succesfully added scene: " + textName.GetText());
			if (loadSceneNamesCallback)
			{
				loadSceneNamesCallback();
			}
			popup.Close();
		}
		else { CONSOLELOG("Failed to add scene: " + textName.GetText()); }
	}
),
closeSceneBtn(150, 30, "Close", [this]()
	{
		popup.Close();
	}
),
loadSceneNamesCallback(lSNC)
{
}

void SceneMenuCreatePopup::Update()
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

SceneMenuDeletePopup::SceneMenuDeletePopup(std::function<void()> deleteCallback) :
	popup("Delete"), deleteCallback(deleteCallback),
	close(130, 30, "Close", [this](void)
		{
			popup.Close();
		}
	),
	confirm(130, 30, "Confirm", [this](void)
		{
			if (this->deleteCallback)
			{
				this->deleteCallback();
			}
			popup.Close();
		}
	)
{
}

void SceneMenuDeletePopup::Update()
{
	if (popup.Begin())
	{
		ImGui::Text("Are you sure you want to delete this scene?");
		close.Update();
		ImGui::SameLine();
		confirm.Update();
		popup.End();
	}
}