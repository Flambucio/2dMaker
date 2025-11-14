#pragma once
#include "../ComponentMenus/ComponentMenus.h"
#include "OpenComponentMenuEvent.h"

namespace D2Maker
{
	namespace GUI
	{
		class ComponentCreation
		{
		private:
			const std::vector<std::string> componentsStrArr = { "Transform","Collider",
				"Velocity","Timer","Name","Audio","Script","Texture",
				"Animation","RigidBody","Camera","Follow" };
			
			Entity& selectedEntity;
			GUIAPI::PopUp popup;
			GUIAPI::Dropdown dropdown;
			GUIAPI::ButtonWithCallback<> closeBtn;
			GUIAPI::ButtonWithCallback<> selectBtn;
			std::function<void()> updateComponents;
			std::unique_ptr<OpenComponentMenuEventUI> &EventPtr;
		public:
			bool createEvent;
			ComponentCreation(Entity& selectedEntity, std::function<void()> updateComponents,
				std::unique_ptr<OpenComponentMenuEventUI>& EventPtr);
			void Update();

			inline void Activate()
			{
				popup.Open();
			}

			inline void CloseCallback()
			{
				popup.Close();
				if (this->updateComponents) 
				{ 
					this->updateComponents(); 
				}
			}
			
		};
	}
}