#include "ComponentsMenuMain.h"
using namespace D2Maker::GUI;

ComponentsMenu::ComponentsMenu(Entity& selectedEntity, std::unordered_map<std::string, Entity>& entitiesBuffer) :
	selectedEntity(selectedEntity), entitiesBuffer(entitiesBuffer), componentList({}, 0, 580, 20),
	componentMenus(selectedEntity, [this]() {this->UpdateBuffers();}),
	createBtn(190, 30, "Add", [this](void)
		{
			if (this->entitiesBuffer.empty()) return;
			componentCreation.Activate();
		}
	),
	deleteBtn(190, 30, "Delete", [this](void)
		{
			if (this->componentNames.empty() || this->entitiesBuffer.empty()) return;
			componentDelete.Activate(componentList.GetCurrentValue());
		}
	),
	modifyBtn(190, 30, "Modify", [this](void)
		{
			if (this->componentNames.empty() || this->entitiesBuffer.empty()) return;
			OCMEUI = std::make_unique<OpenComponentMenuEventUI>(componentList.GetCurrentValue(), true);

		}
	),
	componentCreation(selectedEntity, [this](void) {this->UpdateBuffers();}, OCMEUI),
	componentDelete(selectedEntity, [this](void) {this->UpdateBuffers();}),
	infoBtn(585, 30, "Info", [this](void)
		{
			if (this->componentNames.empty() || this->entitiesBuffer.empty()) return;
			componentInfo.Activate(componentList.GetCurrentValue(), this->selectedEntity);
		}
	)

{

}

void ComponentsMenu::Update()
{
	GUIAPI::GUIWindow::CreateFixedWindow(400, 60, 600, 270, "Components Menu");

	ImGui::Text("Components");
	if (!entitiesBuffer.empty())
	{
		BufferCheck();
		componentList.Update();
	}
	GUIAPI::GUIWindow::EndWindow();
	GUIAPI::GUIWindow::CreateFixedWindow(400, 330, 600, 80, "Components Control");
	modifyBtn.Update();
	ImGui::SameLine();
	createBtn.Update();
	ImGui::SameLine();
	deleteBtn.Update();
	infoBtn.Update();
	componentCreation.Update();
	componentDelete.Update();
	componentInfo.Update();

	if (OCMEUI)
	{
		componentMenus.ActivateComponentWindow(OCMEUI->component, OCMEUI->componentExists);
		OCMEUI = nullptr;
	}
	componentMenus.Update();

	GUIAPI::GUIWindow::EndWindow();
}

void ComponentsMenu::BufferCheck()
{
	if (selectedEntity != selectedEntityBuffer || sceneNameBuffer!= SceneManager::currentScene)
		{
			selectedEntityBuffer = selectedEntity;
			sceneNameBuffer = SceneManager::currentScene;
			UpdateBuffers();
		}
	}

void ComponentsMenu::UpdateBuffers()
{
	//updated the names in the menu as you can see on line 7 in this function
	componentNames.clear();
	for (auto& pair : SceneManager::GetScene(SceneManager::currentScene)->em.GetMap()[selectedEntity])
	{
		std::string componentStr = pair.first.name();
		SliceStringReference(componentStr, 16, componentStr.size() - 1);
		if (componentStr == "TextureComponent") componentStr = "Texture";
		componentNames.push_back(componentStr);
	}
	componentList.UpdateValues(componentNames);
}

ComponentDelete::ComponentDelete(Entity& selectedEntity, std::function<void()> updateComponents) :
	selectedEntity(selectedEntity), updateComponents(updateComponents),
	popup("Delete Component"),
	closeBtn(100, 30, "Close", [this](void)
		{
			popup.Close();
			selectedComponent = "";
		}
	),
	confirmBtn(100, 30, "Confirm", [this](void)
		{
			StartDeletion(selectedComponent);
		}
	)
{
}

void ComponentDelete::StartDeletion(std::string component)
{
	if (component == "Transform")
	{
		DeleteComponent<Transform>();
	}
	else if (component == "Collider")
	{
		DeleteComponent<Collider>();
	}

	else if (component == "Velocity")
	{
		DeleteComponent<Velocity>();
	}

	else if (component == "Timer")
	{
		DeleteComponent<Timer>();
	}

	else if (component == "Audio")
	{
		DeleteComponent<AudioComponent>();
	}

	else if (component == "Script")
	{
		DeleteComponent<Script>();
	}

	else if (component == "Texture")
	{
		DeleteComponent<TextureComponent>();
	}

	else if (component == "Name")
	{
		DeleteComponent<Name>();
	}

	else if (component == "Animation")
	{
		DeleteComponent<Animation>();
	}

	else if (component == "Rigidbody")
	{
		DeleteComponent<RigidBody>();
	}

	else if (component == "Follow")
	{
		DeleteComponent<Follow>();
	}

	else if (component == "Camera")
	{
		DeleteComponent<Camera>();
	}
}

void ComponentDelete::Activate(std::string component)
{
	selectedComponent = component;
	popup.Open();
}

void ComponentDelete::Update()
{
	if (popup.Begin())
	{
		ImGui::Text("Are you sure you want to delete this component?");
		closeBtn.Update();
		ImGui::SameLine();
		confirmBtn.Update();;
		popup.End();
	}
}

void ComponentInfo::GetInfo(std::string component, Entity selectedEntity)
{

	if (component == "Transform")
	{
		Get<Transform>(selectedEntity);
	}
	else if (component == "Velocity")
	{
		Get<Velocity>(selectedEntity);
	}

	else if (component == "Audio")
	{
		Get<AudioComponent>(selectedEntity);
	}

	else if (component == "Script")
	{
		Get<Script>(selectedEntity);
	}

	else if (component == "Texture")
	{
		Get<TextureComponent>(selectedEntity);
	}

	else if (component == "Name")
	{
		Get<Name>(selectedEntity);
	}

	else if (component == "Animation")
	{
		Get<Animation>(selectedEntity);
	}

	else if (component == "RigidBody")
	{
		Get<RigidBody>(selectedEntity);
	}

	else if (component == "Follow")
	{
		Get<Follow>(selectedEntity);
	}

	else if (component == "Camera")
	{
		Get<Camera>(selectedEntity);
	}
	else
	{
		Get<void>(selectedEntity);
	}
}