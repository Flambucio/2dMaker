#pragma once
#include "TransformCreation.h"
#include "ColliderCreation.h"
#include "VelocityCreator.h"
#include "TimerCreator.h"
namespace D2Maker
{
	namespace GUI
	{
		class ComponentCreation
		{
		private:
			const std::vector<std::string> componentsStrArr = { "Transform","Collider",
				"Velocity","Timer","Name","Audio","Script","Texture",
				"Animation","RigidBody","Camera","Follow" };
			std::unordered_map<std::string, std::function<void()>> componentWindowActivation =
			{
				{"Transform",[this](void) {traCreator.Activate();}},
				{"Collider",[this](void) {collCreator.Activate();}},
				{"Velocity",[this](void) {velCreator.Activate();}},
				{"Timer",[this](void) {timCreator.Activate();}},

			
			};
			Entity& selectedEntity;
			GUIAPI::PopUp popup;
			GUIAPI::Dropdown dropdown;
			GUIAPI::ButtonWithCallback<> closeBtn;
			GUIAPI::ButtonWithCallback<> selectBtn;
			TransformCreator traCreator;
			ColliderCreator collCreator;
			VelocityCreator velCreator;
			TimerCreator timCreator;
			std::function<void()> updateComponents;
		public:
			ComponentCreation(Entity& selectedEntity,std::function<void()> updateComponents) : selectedEntity(selectedEntity),
				popup("Add Component"),
				traCreator(selectedEntity, [this](void) {this->CloseCallback();}),
				collCreator(selectedEntity, [this](void) {this->CloseCallback();}),
				velCreator(selectedEntity, [this](void) { this->CloseCallback(); }),
				timCreator(selectedEntity, [this](void) { this->CloseCallback(); }),
				dropdown(componentsStrArr, 0, "Select Component"),
				updateComponents(updateComponents),
				closeBtn(100, 30, "Close", [this](void)
					{
						popup.Close();
					}
				),
				selectBtn(100, 30, "Select", [this](void)
					{
						auto it = componentWindowActivation.find(dropdown.GetCurrVal());
						if (it != componentWindowActivation.end())
						{
							it->second();
						}
					}
				)
			{}

			void Update()
			{
				if (popup.Begin())
				{
					dropdown.Update();
					closeBtn.Update();
					ImGui::SameLine();
					selectBtn.Update();
					traCreator.Update();
					collCreator.Update();
					velCreator.Update();
					timCreator.Update();
					popup.End();
				}
			}

			void Activate()
			{
				popup.Open();
			}

			void CloseCallback()
			{
				popup.Close();
				if (this->updateComponents) 
				{ 
					this->updateComponents(); 
				}
			}
			
		};
	}
}