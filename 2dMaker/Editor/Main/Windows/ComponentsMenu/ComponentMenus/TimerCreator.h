#pragma once
#include "../../../../API/APIExport.h"
namespace D2Maker
{
	namespace GUI
	{
		class TimerCreator
		{
		private:
			Entity& selectedEntity;
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			std::function<void()> updateComponents;
		public:
			TimerCreator(Entity& selectedEntity, std::function<void()> closeCreation);
			void Update();

			inline void Activate(bool componentExists)
			{
				if (componentExists) return;
				popup.Open();
			}

		};
	}
}
