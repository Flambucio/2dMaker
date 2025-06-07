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
			AnimationCreator(Entity& selectedEntity, std::function<void()> closeCreation) : updateComponents(updateComponents),
				popup("Animation"),
				selectedEntity(selectedEntity),
				textBox_Timing("Timing of every sprite"),
				textBox_OIL("Order in Layer"),
				textBox_Names("Texture Names"),
				addBtn(100, 30, "Add/Mod", [this](void)
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
					if (componentExists)
					{
						ImGui::Text("");
						ImGui::Text("WARNING");
						ImGui::Text("This component is not really intended to be modified since is a");
						ImGui::Text("component that relies on external files and not normal values");
					}
					closeBtn.Update();
					ImGui::SameLine();
					addBtn.Update();
					popup.End();
				}
			}

			void Activate(bool componentExists)
			{
				this->componentExists = componentExists;
				if (componentExists)
				{
					Animation* anim = SceneManager::GetScene(SceneManager::currentScene)->em.getComponent<Animation>(selectedEntity);
					std::string namesStr="";
					for (int i = 0;i < anim->texNames.size();i++)
					{
						namesStr += anim->texNames[i];
						if (i != anim->texNames.size() - 1)
						{
							namesStr += ',';
						}
					}
					textBox_Names.SetText(namesStr);
					textBox_OIL.SetText(std::to_string(anim->orderInLayer));
					textBox_Timing.SetText(std::to_string(anim->timing));

				}
				else
				{
					textBox_OIL.SetText(std::to_string(0));
					textBox_Timing.SetText(std::to_string(0));
				}
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
				bool canClose = false;
				Scene* currentScene = SceneManager::GetScene(SceneManager::currentScene);

				if (!componentExists)
				{
					if (currentScene->em.addComponent<Animation>(this->selectedEntity,
						namesVec, timing, oil))
					{
						canClose = true;
					}
				}
				else
				{
					std::vector<std::string> oldNamesVec = {};
					float oldTiming = 0;
					int oldOil=0;
					{
						Animation* anim = currentScene->em.getComponent<Animation>(selectedEntity);
						oldNamesVec = anim->texNames;
						oldTiming = anim->timing;
						oldOil = anim->orderInLayer;
					}
					if (currentScene->em.RemoveComponent<Animation>(selectedEntity))
					{
						if (currentScene->em.addComponent<Animation>(selectedEntity, namesVec, timing, oil))
						{
							canClose = true;
						}
						else
						{
							currentScene->em.addComponent<Animation>(selectedEntity, oldNamesVec, oldTiming, oldOil);
						}
					}

				}

				if (!canClose) return;

				popup.Close();
				if (this->updateComponents)
				{
					this->updateComponents();
				}

			}

		};
	}
}

