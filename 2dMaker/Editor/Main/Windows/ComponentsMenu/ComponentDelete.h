#pragma once
#include "../../../API/APIExport.h"
namespace D2Maker
{
	namespace GUI
	{
		class ComponentDelete
		{
		private:
			Entity& selectedEntity;
			std::function<void()> updateComponents;
			std::string selectedComponent="";
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> closeBtn;
			GUIAPI::ButtonWithCallback<> confirmBtn;
			template<typename T>
			void DeleteComponent()
			{
				if (SceneManager::GetScene(SceneManager::currentScene)->em.RemoveComponent<T>(selectedEntity))
				{
					std::string component = (std::type_index(typeid(T)).name());
					CONSOLELOG("Succesfully removed component: " + component);
					if (updateComponents)
					{
						updateComponents();
					}
					popup.Close();
				}

			}


		public:
			ComponentDelete(Entity& selectedEntity, std::function<void()> updateComponents);

			void StartDeletion(std::string component);
			void Activate(std::string component);

			void Update();




		};
	}
}
