#pragma once
#include "../../../../API/APIExport.h"
namespace D2Maker
{
	namespace GUI
	{
		class TextureComponentCreator
		{
		private:
			Entity& selectedEntity;
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			std::function<void()> updateComponents;
			GUIAPI::TextField textBox_Name;
			GUIAPI::TextField textBox_OIL; // order in layer
			bool componentExists = false;
		public:
			TextureComponentCreator(Entity& selectedEntity, std::function<void()> updateComponents);
			void Update();
			void Activate(bool componentExists);
			void AddTextureComponent();
			  
		};
	}
}


