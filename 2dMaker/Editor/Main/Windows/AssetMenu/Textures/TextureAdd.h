#pragma once
#include "../../../../API/APIExport.h"

namespace D2Maker
{
	namespace GUI
	{
		class TextureAdding
		{
		private:
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			GUIAPI::TextField textBox_path;
			GUIAPI::TextField textBox_name;
			std::function<void()> updateList;
		public:
			TextureAdding(std::function<void()> updateList) : updateList(updateList),
				addBtn(160, 30, "Add", [this](void)
					{
						if (this->AddTexture())
						{
							if (this->updateList)
							{
								this->updateList();
							}
							popup.Close();
						}
					}
				),
				closeBtn(160, 30, "Close", [this](void)
					{
						popup.Close();
					}
				),
				textBox_name("Name"),
				textBox_path("Path"),
				popup("Load Texture")
			{ }

			void Update()
			{
				if (popup.Begin())
				{
					textBox_name.Update();
					textBox_path.Update();
					ImGui::Text("The directory starts at ");
					ImGui::Text("Projects/{currentProject}/Resources/Textures/");
					ImGui::Text("subdirectories are not supported (yet)");
					ImGui::Text("insert only name of the file without extension");
					ImGui::Text("file should be png");
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

			bool AddTexture()
			{
				std::string basePath = "Projects/" + FileSys::currentProject + "/Resources/Textures/";
				std::string path = basePath+textBox_path.GetText()+".png";
				std::string name = textBox_name.GetText();
				if (!fs::exists(path) || !fs::is_regular_file(path))
				{
					return false;
				}
				if (TextureLoader::Exists(name)) return false;
				TextureLoader::LoadTexture(name, path);
				CONSOLELOG("Texture loaded at: " + path + " at: " + name);
				return true;
			}

		};
	}
}