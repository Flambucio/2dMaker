#pragma once
#include "../../../API/APIExport.h"

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
			
		public:
			ComponentsMenu(Entity& selectedEntity, std::unordered_map<std::string, Entity>& entitiesBuffer) :
				selectedEntity(selectedEntity),entitiesBuffer(entitiesBuffer),componentList({},0,580,20)
			{
			}

			void Update()
			{
				GUIAPI::GUIWindow::CreateFixedWindow(400, 60, 600, 270, "Components Menu");
				ImGui::Text("Components");
				if (!entitiesBuffer.empty())
				{
					UpdateBuffers();
					componentList.Update();
				}
				GUIAPI::GUIWindow::EndWindow();
			}

		private:
			void UpdateBuffers()
			{	
				if (selectedEntity != selectedEntityBuffer)
				{
					selectedEntityBuffer = selectedEntity;
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
			}
		};




	}
}