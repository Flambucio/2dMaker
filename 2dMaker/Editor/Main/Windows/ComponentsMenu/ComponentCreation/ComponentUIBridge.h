#pragma once
#include "ComponentCreation.h"
#include "../ComponentMenus/ComponentMenus.h"
namespace D2Maker
{
	namespace GUI
	{
		class ComponentUIBridge
		{
		public:
			ComponentUIBridge(ComponentMenus& menu, ComponentCreation& create)
			{
				(*this)(menu, create);
			}

		private:
			void operator()(ComponentMenus& menu, ComponentCreation& create)
			{
				menu.SetCloseCallback([&create](void) {create.CloseCallback();});
				create.SetFindCallback([&menu](std::string compName, bool componentExists = false)
					{menu.ActivateComponentWindow(compName, componentExists);});
			}

		};
	}
}
