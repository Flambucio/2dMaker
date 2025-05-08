#pragma once
#include "../Application/SceneManager.h"

namespace D2Maker
{
	namespace fs = std::filesystem;
	class FileSys
	{
	public:
		static std::string currentProject;
		static std::unordered_set<std::string> projectNames;
		
		static void CreateProject(const std::string&name)
		{
			if (fs::exists("Projects/" + name))
			{
				WARN("This project already exists");
				return;
			}

			fs::create_directory("Projects/" + name);
			fs::create_directory("Projects/" + name + "/Scripts");
			fs::create_directory("Projects/" + name + "/Scenes");
			fs::create_directories ("Projects/" + name + "/Resources");
			fs::create_directories("Projects/" + name + "/Resources/Assets");
			fs::create_directories("Projects/" + name + "/Resources/AssetsInfo");
			



		}
		static void GetProjects()
		{
			for (const auto& entry : fs::recursive_directory_iterator("Projects")) 
			{
				if (entry.is_directory())
				{
					projectNames.insert(entry.path().string());
				}
			}
		}
		static void DeleteProject(const std::string&name)
		{
			if (!fs::exists("Projects/" + name) || !fs::is_directory("Projects/"+name))
			{
				return;
			}

			fs::remove_all("Projects/" + name);

			auto it = projectNames.find(name);
			if (it != projectNames.end())
			{
				projectNames.erase(it);
			}


			
		}
		static void SelectProject(const std::string&name)
		{
			if (fs::exists("Projects/" + name) && fs::is_directory("Projects/" + name) && currentProject=="")
			{
				currentProject = name;
			}
		}
		static void LoadScenes()
		{
			if (currentProject == "")
			{
				return;
			}

			if (!(fs::exists("Projects/" + currentProject) && fs::is_directory("Projects/" + currentProject)))
			{
				return;
			}

			for (const auto& entry : fs::recursive_directory_iterator("Projects/"+currentProject+"/Scenes"))
			{
				if (entry.is_directory())
				{
					std::string sceneToCreateName = entry.path().filename().string();
					std::string sceneToCreatePath = entry.path().string();
					SceneManager::AddScene(sceneToCreateName);
					for (const auto& entityFile : fs::recursive_directory_iterator(entry.path()))
					{
						LoadEntity(sceneToCreateName, entityFile.path().string());
					}
				}
			}

			


		}
		static void LoadEntity(const std::string& sceneToCreateName, const std::string& EntityPath)
		{
			Scene* scene = SceneManager::GetScene(sceneToCreateName);
			if (!scene) return;
			std::vector<std::vector<std::string>> parsedStr;
			Parser::ParseString(EntityPath, parsedStr);
			Entity entity = scene->em.createEntity(parsedStr[1][0]);

			for (int i = 0;i < parsedStr.size();i++)
			{
				if (parsedStr[i][0] == "TRANSFORM")
				{
					scene->em.addComponent<Transform>(entity,
						stof(parsedStr[i][1]),
						stof(parsedStr[i][2]),
						stof(parsedStr[i][3]),
						stof(parsedStr[i][4]),
						stof(parsedStr[i][5])
					);
				}
				else if (parsedStr[i][0] == "VELOCITY")
				{
					scene->em.addComponent<Velocity>(entity,
						stof(parsedStr[i][1]),
						stof(parsedStr[i][2]),
						stof(parsedStr[i][3])
					);
				}
				else if (parsedStr[i][0] == "COLLIDER")
				{
					scene->em.addComponent<Collider>(entity);
				}
				else if (parsedStr[i][0] == "TIMER")
				{
					scene->em.addComponent<Timer>(entity);
				}
				else if (parsedStr[i][0] == "NAME")
				{
					scene->em.addComponent<Name>(entity, parsedStr[i][1]);
				}
				else if (parsedStr[i][0] == "AUDIO")
				{
					scene->em.addComponent<Audio>(entity, parsedStr[i][1]);
				}
				else if (parsedStr[i][0] == "SCRIPT")
				{
					scene->em.addComponent<Script>(entity, parsedStr[i][1]);
				}
				else if (parsedStr[i][0] == "TEXTURE")
				{
					scene->em.addComponent<TextureComponent>(entity,
						parsedStr[i][1],
						stoi(parsedStr[i][2])
					);
				}
				else if (parsedStr[i][0] == "ANIMATION")
				{
					std::vector<std::string> names;
					for (int j = 1;j < parsedStr[i].size() - 3;j++)
					{
						names.push_back(parsedStr[i][j]);
					}
					scene->em.addComponent<Animation>(entity,
						names,
						stof(parsedStr[i][parsedStr[i].size() - 2]),
						stoi(parsedStr[i][parsedStr[i].size() - 1])
					);
				}
				else if (parsedStr[i][0] == "RIGIDBODY")
				{
					scene->em.addComponent<RigidBody>(entity,
						stof(parsedStr[i][1]),
						stof(parsedStr[i][2]),
						stof(parsedStr[i][3])
					);
				}
				else if (parsedStr[i][0] == "CAMERA")
				{
					bool enableX;
					bool enableY;
					parsedStr[i][1] == "T" ? enableX = true : enableX = false;
					parsedStr[i][2] == "T" ? enableY = true : enableY = false;
					scene->em.addComponent<Camera>(entity,
						enableX,
						enableY,
						stoi(parsedStr[i][3]),
						stoi(parsedStr[i][4])
					);
				}
			}
		}
		static void Save()
		{

		}
	};
}
