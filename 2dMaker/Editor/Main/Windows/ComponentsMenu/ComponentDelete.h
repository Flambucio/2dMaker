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
					if (updateComponents)
					{
						updateComponents();
					}
					popup.Close();
				}
			}


		public:
			ComponentDelete(Entity& selectedEntity, std::function<void()> updateComponents) :
				selectedEntity(selectedEntity), updateComponents(updateComponents),
				popup("Delete Component"),
				closeBtn(100, 30,"Close", [this](void)
					{
						popup.Close();
						selectedComponent = "";
					}
				),
				confirmBtn(100, 30, "Confirm", [this](void)
					{
						StartDeletion(selectedComponent);
					}
				)
			{ }

			void StartDeletion(std::string component)
			{
				if (component == "Transform")
				{
					DeleteComponent<Transform>();
				}
				else if (component == "Collider")
				{
					DeleteComponent<Collider>();
				}

				else if (component == "Velocity")
				{
					DeleteComponent<Velocity>();
				}

				else if (component == "Timer")
				{
					DeleteComponent<Timer>();
				}

				else if (component == "Audio")
				{
					DeleteComponent<AudioComponent>();
				}

				else if (component == "Script")
				{
					DeleteComponent<Script>();
				}

				else if (component == "Texture")
				{
					DeleteComponent<TextureComponent>();
				}

				else if (component == "Name")
				{
					DeleteComponent<Name>();
				}

				else if (component == "Animation")
				{
					DeleteComponent<Animation>();
				}

				else if (component == "Rigidbody")
				{
					DeleteComponent<RigidBody>();
				}

				else if (component == "Follow")
				{
					DeleteComponent<Follow>();
				}

				else if (component == "Camera")
				{
					DeleteComponent<Camera>();
				}
			}

			void Activate(std::string component)
			{
				selectedComponent = component;
				popup.Open();
			}

			void Update()
			{
				if (popup.Begin())
				{
					ImGui::Text("Are you sure you want to delete this component?");
					closeBtn.Update();
					confirmBtn.Update();
					popup.End();
				}
			}




		};
	}
}
