#pragma once
#include "EntityCreate.h"
#include "EntityModify.h"
#include "EntityDelete.h"

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
			EntityModifyPopup eModPopup;
			EntityDeletePopup eDelPopup;
		public:
			EntityMenu() : entityList({}, 0, 385, 20), eDelPopup([this]() {this->DeleteCallback();}),
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
						entityList.UpdateValues({});
						nEntitiesBuffer = 0;
						selectedEntity = 0;
						entitiesBuffer.clear();
						
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

			void Update()
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
				if (!entitiesBuffer.empty() && SceneManager::currentScene!="")
				{
					Scene* scene = SceneManager::GetScene(SceneManager::currentScene);
					std::string type = "";
					TRACE(selectedEntity);
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

			std::unordered_map<std::string, Entity> &GetEntityBuffer()
			{ 
				return entitiesBuffer;
			}

			Entity& GetSelectedEntity()
			{
				return selectedEntity;
			}

		
			void DeleteCallback()
			{
				Scene* scene = SceneManager::GetScene(SceneManager::currentScene);
				scene->em.destroyEntity(entityList.GetCurrentValue());
			}
		};
	}
}

