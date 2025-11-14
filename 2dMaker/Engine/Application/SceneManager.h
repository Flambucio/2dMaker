#pragma once
#include "Scene.h"

namespace D2Maker
{
	class SceneManager
	{
	private:
		
		
		static GLFWwindow* window;
	public:
		static std::string currentScene;
		static std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;
		static std::string defaultScene;
		inline static void InitManager(GLFWwindow * windowIn)
		{
			window = windowIn;
		}
		inline static bool Exists(const std::string& name)
		{
			return scenes.find(name) != scenes.end();
		}
		static void SelectScene(const std::string& name);
		static bool AddScene(const std::string& name);
		static void RemoveScene(const std::string& name);
		static void UpdateCurrentScene(GLFWwindow* window, bool runGameFlag);
		static Scene* GetScene(const std::string& name);

		


	};
}
