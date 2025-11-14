#pragma once
#include "../../../../API/APIExport.h"
namespace D2Maker
{
	namespace GUI
	{
		class ColliderCreator
		{
		private:
			Entity& selectedEntity;
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			std::function<void()> updateComponents;
		public:
			ColliderCreator(Entity& selectedEntity, std::function<void()> updateComponents);
			void Update();
			inline void Activate(bool componentExists)
			{
				if (componentExists) return;
				popup.Open();
			}

		};
	}
}
