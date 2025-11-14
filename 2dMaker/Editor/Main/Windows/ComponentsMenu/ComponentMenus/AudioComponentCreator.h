#pragma once
#include "../../../../API/APIExport.h"
namespace D2Maker
{
	namespace GUI
	{
		class AudioComponentCreator
		{
		private:
			Entity& selectedEntity;
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			std::function<void()> updateComponents;
			GUIAPI::TextField textBox;
			bool componentExists = false;
		public:
			AudioComponentCreator(Entity& selectedEntity, std::function<void()> closeCreation);
			void Update();
			void AddAudioComponent();
			void Activate(bool componentExists);

		};
	}
}

