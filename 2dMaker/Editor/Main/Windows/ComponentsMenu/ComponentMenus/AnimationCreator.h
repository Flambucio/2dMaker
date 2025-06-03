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
			std::function<void()> closeCreation;
			GUIAPI::TextField textBox_Names;
			GUIAPI::TextField textBox_OIL;
			GUIAPI::TextField textBox_Timing;
		public:
			AnimationCreator(Entity& selectedEntity, std::function<void()> closeCreation) : closeCreation(closeCreation),
				popup("Add - Animation"),
				selectedEntity(selectedEntity),
				textBox_Timing("Timing of every sprite"),
				textBox_OIL("Order in Layer"),
				textBox_Names("Texture Names"),
				addBtn(100, 30, "Add", [this](void)
					{
						AddAnimationComponent();
					}
				),
				closeBtn(100, 30, "Close", [this](void)
					{
						popup.Close();
					}
				)
			{
			}

			void Update()
			{
				if (popup.Begin())
				{
					textBox_Names.Update();
					textBox_OIL.Update();
					textBox_Timing.Update();
					ImGui::Text("names shoul be inserted separaed by comma");
					ImGui::Text("Example:");
					ImGui::Text("texture_1,texture_2");
					ImGui::Text("names cant contain spaces (yet)");
					closeBtn.Update();
					ImGui::SameLine();
					addBtn.Update();
					popup.End();
				}
			}

			void Activate()
			{
				popup.Open();
			}

			void AddAnimationComponent()
			{
				int oil=0;
				if (!ConvertStringToNum<int>(textBox_OIL.GetText(), oil)) return;
				
				float timing = 0;
				if (!ConvertStringToNum<float>(textBox_Timing.GetText(), timing)) return;

				std::string unparsedNames = textBox_Names.GetText();
				std::unordered_set<std::string> names;
				std::string tStr="";
				for (int i = 0;i < unparsedNames.size();i++)
				{
					char tChar = unparsedNames[i];
					if (tChar == ',')
					{
						if (tStr == "") return;
						names.insert(tStr);
						tStr = "";
					}
					else
					{
						tStr += tChar;
					}

				}
				names.insert(tStr);
				std::vector<std::string> namesVec;
				SetToVector(names, namesVec);

				if (SceneManager::GetScene(SceneManager::currentScene)->em.addComponent<Animation>(this->selectedEntity,
					namesVec,timing,oil))
				{
					popup.Close();
					if (this->closeCreation)
					{
						this->closeCreation();
					}
				}


			}

		};
	}
}

