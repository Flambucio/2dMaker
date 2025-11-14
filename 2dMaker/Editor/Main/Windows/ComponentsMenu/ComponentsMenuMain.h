#pragma once
#include "ComponentCreation/ComponentCreation.h"
#include "ComponentDelete.h"
#include "ComponentInfo.h"

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
			GUIAPI::ButtonWithCallback<> infoBtn;
			
			ComponentMenus componentMenus;
			ComponentCreation componentCreation;
			ComponentDelete componentDelete;
			ComponentInfo componentInfo;
			std::unique_ptr<OpenComponentMenuEventUI> OCMEUI = nullptr;
			
			
		public:
			ComponentsMenu(Entity& selectedEntity, std::unordered_map<std::string, Entity>& entitiesBuffer);
			void Update();
		private:
			void BufferCheck();
			void UpdateBuffers();
		};




	}
}