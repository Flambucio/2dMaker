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
			EntityMenu();
			void Update();
			void UpdateBuffers();

			inline std::unordered_map<std::string, Entity> &GetEntityBuffer()
			{ 
				return entitiesBuffer;
			}

			inline Entity& GetSelectedEntity()
			{
				return selectedEntity;
			}

			void DeleteCallback();
		};
	}
}

