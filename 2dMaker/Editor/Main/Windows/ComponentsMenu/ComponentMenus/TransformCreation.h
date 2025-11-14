#pragma once
#include "../../../../API/APIExport.h"

namespace D2Maker
{
	namespace GUI
	{
		class TransformCreator
		{
		private:
			Entity& selectedEntity;
			GUIAPI::TextField textBox_x;
			GUIAPI::TextField textBox_y;
			GUIAPI::TextField textBox_width;
			GUIAPI::TextField textBox_height;
			GUIAPI::TextField textBox_rotation;
			GUIAPI::ButtonWithCallback<> closeBtn;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::PopUp popup;
			std::function<void()> updateComponents;
			bool componentExists = false;

		public:

			TransformCreator(Entity& selectedEntity, std::function<void()> updateComponents);
			void Update();
			void Activate(bool componentExists);
			void AddTransformComponent();
	


		};
	}
}
