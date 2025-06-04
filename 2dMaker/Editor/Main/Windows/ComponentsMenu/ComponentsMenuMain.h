#pragma once
#include "ComponentCreation/ComponentCreation.h"

namespace D2Maker
{
	namespace GUI
	{
		class ComponentsMenu
		{
		private:
			
			Entity& selectedEntity;
			Entity selectedEntityBuffer=999;
			std::unordered_map<std::string,Entity>& entitiesBuffer;
			std::vector<std::string> componentNames = {};
			GUIAPI::Menu componentList;
			
			GUIAPI::ButtonWithCallback<> createBtn;
			GUIAPI::ButtonWithCallback<> deleteBtn;
			GUIAPI::ButtonWithCallback<> modifyBtn;
			ComponentMenus componentMenus;
			ComponentCreation componentCreation;
			std::unique_ptr<OpenComponentMenuEventUI> OCMEUI = nullptr;
			
			
		public:
			ComponentsMenu(Entity& selectedEntity, std::unordered_map<std::string, Entity>& entitiesBuffer) :
				selectedEntity(selectedEntity), entitiesBuffer(entitiesBuffer), componentList({}, 0, 580, 20),
				componentMenus(selectedEntity),
				createBtn(190, 30, "Add", [this](void)
					{
						componentCreation.Activate();
					}
				),
				deleteBtn(190, 30, "Delete", [this](void)
					{

					}
				),
				modifyBtn(190, 30, "Modify", [this](void)
					{
						OCMEUI = std::make_unique<OpenComponentMenuEventUI>(componentList.GetCurrentValue(), true);

					}
				),
				componentCreation(selectedEntity, [this](void) {this->UpdateBuffers();},OCMEUI)
				
			{
				
			}

			void Update()
			{
				GUIAPI::GUIWindow::CreateFixedWindow(400, 60, 600, 270, "Components Menu");
				
				ImGui::Text("Components");
				if (!entitiesBuffer.empty())
				{
					BufferCheck();
					componentList.Update();
				}
				GUIAPI::GUIWindow::EndWindow();
				GUIAPI::GUIWindow::CreateFixedWindow(400, 330, 600,300, "Components Control");
				modifyBtn.Update();
				ImGui::SameLine();
				createBtn.Update();
				ImGui::SameLine();
				deleteBtn.Update();
				componentCreation.Update();

				if (OCMEUI)
				{
					componentMenus.ActivateComponentWindow(OCMEUI->component, OCMEUI->componentExists);
					OCMEUI = nullptr;
				}
				componentMenus.Update();
				
				GUIAPI::GUIWindow::EndWindow();
			}

		private:
			void BufferCheck()
			{	
				if (selectedEntity != selectedEntityBuffer)
				{
					selectedEntityBuffer = selectedEntity;
					UpdateBuffers();
				}
			}

			void UpdateBuffers()
			{
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

			


		};




	}
}