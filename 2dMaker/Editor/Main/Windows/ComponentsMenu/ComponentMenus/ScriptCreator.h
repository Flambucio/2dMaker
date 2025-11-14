#pragma once
#include "../../../../API/APIExport.h"
namespace D2Maker
{
	namespace GUI
	{
		class ScriptCreator
		{
		private:
			Entity& selectedEntity;
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			std::function<void()> updateComponents;
			GUIAPI::TextField textBox;
			bool componentExists=false;
			
		public:
			ScriptCreator(Entity& selectedEntity, std::function<void()> updateComponents);

			void Update();
			void Activate(bool componentExists);
			void AddScriptComponent();
			

				

		};
	}
}
