#include "EntityMenu.h"
using namespace D2Maker::GUI;

EntityCreatePopup::EntityCreatePopup() :
	popup("Create Entity"),
	close(100, 30, "Close", [this](void)
		{
			popup.Close();
		}
	),
	create(100, 30, "Create", [this](void)
		{
			Scene* currentScene = SceneManager::GetScene(SceneManager::currentScene);
			EntityType type = EntityType::NORMAL;
			if (currentScene->em.nameAvailable(this->textBox.GetText()))
			{
				if (this->checkbox.GetValue()) type = EntityType::VIRTUAL;
				currentScene->em.createEntity(this->textBox.GetText(), type);
				popup.Close();
				CONSOLELOG("Created entity: " + textBox.GetText());
			}
		}
	),
	textBox("Entity Name"),
	checkbox("Is Virtual?")

{
}

void EntityCreatePopup::Update()
{
	if (popup.Begin())
	{
		textBox.Update();
		checkbox.Update();
		close.Update();
		ImGui::SameLine();
		create.Update();
		popup.End();
	}
}

EntityDeletePopup::EntityDeletePopup(std::function<void()> deleteCallback,
	std::function<void()> updateCallback) :
	popup("Delete"), deleteCallback(deleteCallback), updateCallback(updateCallback),
close(100, 30, "Close", [this]()
	{
		popup.Close();
	}
),
confirm(100, 30, "Confirm", [this]()
	{
		if (this->deleteCallback)
		{
			this->deleteCallback();
			if (this->updateCallback) this->updateCallback();
			this->popup.Close();
		}


	}
)
{
}

void EntityDeletePopup::Update()
{
	if (popup.Begin())
	{
		ImGui::Text("Are you sure you want to delete this entity?");
		close.Update();
		ImGui::SameLine();
		confirm.Update();
		popup.End();
	}
}

EntityModifyPopup::EntityModifyPopup() : textBox("New Name"), popup("Modify Entity"),
closeBtn(100, 30, "Close", [this](void)
	{
		popup.Close();
	}
),
confirmBtn(100, 30, "Confirm", [this](void)
	{
		if (this->textBox.GetText() == "") return;
		if (SceneManager::GetScene(SceneManager::currentScene)->
			em.renameEntity(currentEntityName, this->textBox.GetText()))
		{
			popup.Close();
		}
	}
)
{
}

void EntityModifyPopup::Update()
{
	if (popup.Begin())
	{
		textBox.Update();
		closeBtn.Update();
		ImGui::SameLine();
		confirmBtn.Update();
		popup.End();
	}
}


EntityMenu::EntityMenu() : entityList({}, 0, 385, 20), eDelPopup([this](void) {this->DeleteCallback();}
, [this](void) {this->UpdateBuffers();}),
createBtn(120, 50, "Create", [this](void)
	{
		if (SceneManager::currentScene != "")
		{
			this->eCreatePopup.Activate();
		}


	}
),
deleteBtn(120, 50, "Delete", [this](void)
	{
		if (!entitiesBuffer.empty())
		{
			this->eDelPopup.Activate();
		}
		TRACE("updating values");
		//entityList.UpdateValues({});

	}
),
modify(120, 50, "Modify", [this](void)
	{
		std::string currEntityStr = entityList.GetCurrentValue();
		if (currEntityStr != "" && !entitiesBuffer.empty())
		{
			TRACE(currEntityStr + "CURRent");
			eModPopup.Activate(currEntityStr);
		}
	}
),
drag(380, 30, "Drag (Coming Soon)", [this](void)
	{

	}
)
{
}

void EntityMenu::Update()
{
	UpdateBuffers();
	GUIAPI::GUIWindow::CreateFixedWindow(0, 60, 400, 540, "Entities");
	ImGui::Text("Entities");
	if (!entitiesBuffer.empty())
	{
		entityList.Update();
		//if (entitiesBuffer[entityList.GetCurrentValue()] != selectedEntity)
		//{
		selectedEntity = entitiesBuffer[entityList.GetCurrentValue()];
		//}
	}
	GUIAPI::GUIWindow::EndWindow();
	GUIAPI::GUIWindow::CreateFixedWindow(0, 600, 400, 110, "Entity Controls");
	modify.Update();
	ImGui::SameLine();
	createBtn.Update();
	ImGui::SameLine();
	deleteBtn.Update();
	drag.Update();
	eCreatePopup.Update();
	eModPopup.Update();
	eDelPopup.Update();
	GUIAPI::GUIWindow::EndWindow();
	GUIAPI::GUIWindow::CreateFixedWindow(401, 600, 600, 120, "Entity Info");
	if (!entitiesBuffer.empty() && SceneManager::currentScene != "")
	{
		Scene* scene = SceneManager::GetScene(SceneManager::currentScene);
		std::string type = "";
		//TRACE(selectedEntity);
		scene->em.isVirtualEntity(selectedEntity) ? type = "Virtual" : type = "Regular";
		std::string entityInfo_Name = "Entity Name: " + entityList.GetCurrentValue();
		std::string entityInfo_Type = "Entity Type: " + type;
		ImGui::Text("Entity Info");
		ImGui::Text("");
		ImGui::Text(entityInfo_Name.c_str());
		ImGui::Text("");
		ImGui::Text(entityInfo_Type.c_str());

	}
	GUIAPI::GUIWindow::EndWindow();
}


void EntityMenu::UpdateBuffers()
{
	if (SceneManager::currentScene == "")
	{
		nEntitiesBuffer = 0;
		selectedEntity = 0;
		entitiesBuffer.clear();
		entityList.UpdateValues({});
		return;
	}
	Scene* currentScene = SceneManager::GetScene(SceneManager::currentScene);
	if (nEntitiesBuffer != currentScene->em.aliveEntities.size() ||
		SceneManager::currentScene != sceneNameBuffer)
	{
		nEntitiesBuffer = currentScene->em.aliveEntities.size();
		entitiesBuffer.clear();
		std::vector<std::string> names;
		for (auto& pair : currentScene->em.entityNames)
		{
			entitiesBuffer[pair.first] = pair.second;
			names.push_back(pair.first);
		}

		entityList.UpdateValues(names);
	}
}

void EntityMenu::DeleteCallback()
{
	Scene* scene = SceneManager::GetScene(SceneManager::currentScene);
	CONSOLELOG("Deleted entity: " + entityList.GetCurrentValue());
	scene->em.destroyEntity(entityList.GetCurrentValue());
}