#pragma once
#include "../../../../API/APIExport.h"
namespace D2Maker
{
	namespace GUI
	{
		class AnimationCreator
		{
		private:
			Entity& selectedEntity;
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			std::function<void()> updateComponents;
			GUIAPI::TextField textBox_Names;
			GUIAPI::TextField textBox_OIL;
			GUIAPI::TextField textBox_Timing;
			bool componentExists = false;
		public:
			AnimationCreator(Entity& selectedEntity, std::function<void()> closeCreation);
			void Update();
			void Activate(bool componentExists);
			void AddAnimationComponent();

		};
	}
}

