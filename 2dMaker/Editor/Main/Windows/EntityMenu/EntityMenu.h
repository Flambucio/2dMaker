#pragma once
#include "EntityCreate.h"

namespace D2Maker
{
	namespace GUI
	{
		class EntityMenu
		{
		private:
			//normal entities
			std::unordered_map<std::string, Entity> entitiesBuffer;
			GUIAPI::Menu entityList;
			//all
			unsigned int nEntitiesBuffer = 0;
			Entity selectedEntity=0;
			std::string sceneNameBuffer = "";

			GUIAPI::ButtonWithCallback<> createBtn;
			GUIAPI::ButtonWithCallback<> deleteBtn;
			GUIAPI::ButtonWithCallback<> modify;
			GUIAPI::ButtonWithCallback<> drag;

			EntityCreatePopup eCreatePopup;
		public:
			EntityMenu() : entityList({}, 0, 385, 20),
				createBtn(120, 50, "Create", [this](void)
					{
						this->eCreatePopup.Activate();
					}
				),
				deleteBtn(120, 50, "Delete", [this](void)
					{

					}
				),
				modify(120, 50, "Modify", [this](void)
					{

					}
				),
				drag(380, 30, "Drag (Coming Soon)", [this](void)
					{

					}
				)
			{
			}

			void Update()
			{
				UpdateBuffers();
				GUIAPI::GUIWindow::CreateFixedWindow(0, 60, 400, 540, "Entities");
				ImGui::Text("Entities");
				if (!entitiesBuffer.empty())
				{
					entityList.Update();
					if (entitiesBuffer[entityList.GetCurrentValue()] != selectedEntity)
					{
						selectedEntity = entitiesBuffer[entityList.GetCurrentValue()];
					}
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
				GUIAPI::GUIWindow::EndWindow();
				GUIAPI::GUIWindow::CreateFixedWindow(401, 600, 600, 120, "Entity Info");
				if (!entitiesBuffer.empty())
				{
					Scene* scene = SceneManager::GetScene(SceneManager::currentScene);
					std::string type = "";
					scene->em.isVirtualEntity(selectedEntity) ? type = "Virtual" : type = "Regular";
					std::string entityInfo_Name= "Entity Name: " + entityList.GetCurrentValue();
					std::string entityInfo_Type = "Entity Type: " + type;
					ImGui::Text("Entity Info");
					ImGui::Text("");
					ImGui::Text(entityInfo_Name.c_str());
					ImGui::Text("");
					ImGui::Text(entityInfo_Type.c_str());
					
				}
				GUIAPI::GUIWindow::EndWindow();
			}


			void UpdateBuffers()
			{
				if (SceneManager::currentScene == "") return;
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

			std::unordered_map<std::string, Entity> &GetEntityBuffer()
			{ 
				return entitiesBuffer;
			}

			Entity& GetSelectedEntity()
			{
				return selectedEntity;
			}
		};
	}
}

