#pragma once
#include "../../../../API/APIExport.h"
namespace D2Maker
{
	namespace GUI
	{
		class RigidbodyCreator
		{
		private:
			Entity& selectedEntity;
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			std::function<void()> updateComponents;
			GUIAPI::TextField textBox_Mass;
			GUIAPI::TextField textBox_BounceFactor;
			GUIAPI::TextField textBox_MaxDy;
			bool componentExists = false;
		public:
			RigidbodyCreator(Entity& selectedEntity, std::function<void()> closeCreation);
			void Update();
			void Activate(bool componentExists);
			void AddRigidBodyComponent();

		};
	}
}

