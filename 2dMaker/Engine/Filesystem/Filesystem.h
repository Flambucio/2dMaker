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
		
		static bool CreateProject(const std::string& name);
		static void GetProjects();
		static void DeleteProject(const std::string& name);
		static void SelectProject(const std::string& name);
		static void LoadScenes();
		static void LoadEntity(const std::string& sceneToCreateName, const std::string& EntityPath);
		static void Save();
		static void SaveEntity(EntityManager& em, std::pair<std::string, Entity> entityPair, std::string currentSceneDirectory);
		static void LoadAssets();
		static void LoadAudios();
		static void LoadTextures();
		static void SaveAssets();
		static void SaveTextures();
		static void SaveAudios();
		static void LoadConfigs();
		static void SaveConfigs();
	};
}
