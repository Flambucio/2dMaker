#pragma once
#include "../../../../API/APIExport.h"

namespace D2Maker
{
	namespace GUI
	{
		class VelocityCreator
		{
		private:
			Entity& selectedEntity;
			std::function<void()> updateComponents;
			
			GUIAPI::TextField textBox_dx;
			GUIAPI::TextField textBox_dy;
			GUIAPI::TextField textBox_dtheta;

			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;

			GUIAPI::PopUp popup;

			bool componentExists=false;
		public:
			VelocityCreator(Entity& selectedEntity, std::function<void()> updateComponents);
			void Update();
			void AddVelocityComponent();
			void Activate(bool componentExists);


			



		};
	}
}
