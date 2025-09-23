#pragma once
#include "../../../../API/APIExport.h"

namespace D2Maker
{
	namespace GUI
	{
		class AudioAdding
		{
		private:
			GUIAPI::PopUp popup;
			GUIAPI::ButtonWithCallback<> addBtn;
			GUIAPI::ButtonWithCallback<> closeBtn;
			GUIAPI::TextField textBox_path;
			GUIAPI::TextField textBox_name;
			std::function<void()> updateList;
		public:
			AudioAdding(std::function<void()> updateList) : updateList(updateList),
				addBtn(160, 30, "Add", [this](void)
					{
						if (this->AddAudio())
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
				popup("Load Audio")
			{
			}

			void Update()
			{
				if (popup.Begin())
				{
					textBox_name.Update();
					textBox_path.Update();
					ImGui::Text("The directory starts at ");
					ImGui::Text("Projects/{currentProject}/Resources/Audios/");
					ImGui::Text("subdirectories are not supported (yet)");
					ImGui::Text("insert only name of the file without extension");
					ImGui::Text("file should be ogg");
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

			bool AddAudio()
			{
				std::string basePath = "Projects/" + FileSys::currentProject + "/Resources/Audios/";
				std::string path = basePath + textBox_path.GetText() + ".ogg";
				std::string name = textBox_name.GetText();
				if (!fs::exists(path) || !fs::is_regular_file(path))
				{
					return false;
				}
				if (AudioLoader::Exists(name)) return false;
				AudioLoader::LoadAudio(name, path);
				CONSOLELOG("Loaded audio at : " + path + " as: " + name);
				return true;
			}

		};
	}
}
