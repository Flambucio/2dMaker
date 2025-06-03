#pragma once

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
			
			Entity& selectedEntity;
			GUIAPI::PopUp popup;
			GUIAPI::Dropdown dropdown;
			GUIAPI::ButtonWithCallback<> closeBtn;
			GUIAPI::ButtonWithCallback<> selectBtn;
			std::function<void()> updateComponents;
			std::function<void(std::string,bool)> find;
		public:
			ComponentCreation(Entity& selectedEntity,std::function<void()> updateComponents) : selectedEntity(selectedEntity),
				popup("Add Component"),

				dropdown(componentsStrArr, 0, "Select Component"),
				updateComponents(updateComponents),
				closeBtn(100, 30, "Close", [this](void)
					{
						popup.Close();
					}
				),
				selectBtn(100, 30, "Select", [this](void)
					{
						//TRACE("select");
						if (find)
						{
							//TRACE("find func found!");
							this->find(dropdown.GetCurrVal(),false);
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


			void SetFindCallback(std::function<void(std::string,bool)> f)
			{
				find = f;
			}
			
		};
	}
}