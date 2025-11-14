#pragma once
#include "../../../../API/APIExport.h"
namespace D2Maker
{
	namespace GUI
	{
		class CameraCreator
		{
		private:
			Entity& selectedEntity;
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			std::function<void()> closeCreation;
			GUIAPI::TextField textBox_x;
			GUIAPI::TextField textBox_y;
			GUIAPI::CheckBox checkBox_enableX;
			GUIAPI::CheckBox checkBox_enableY;
			bool componentExists = false;
			
		public:
			CameraCreator(Entity& selectedEntity, std::function<void()> closeCreation);
			void Update();
			void Activate(bool componentExists);
			void AddCameraComponent();

		};
	}
}

