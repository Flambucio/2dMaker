#pragma once
#include "../../../../API/APIExport.h"
namespace D2Maker
{
	namespace GUI
	{
		class FollowCreator
		{
		private:
			Entity& selectedEntity;
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			std::function<void()> closeCreation;
			GUIAPI::TextField textBox_NameToFollow;
			GUIAPI::TextField textBox_Vel;
			bool componentExists = false;
		public:
			FollowCreator(Entity& selectedEntity, std::function<void()> closeCreation);
			void Update();
			void Activate(bool componentExists);
			void AddFollowComponent();

		};
	}
}
