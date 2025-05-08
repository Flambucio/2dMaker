#pragma once

#include "Scene.h"

namespace D2Maker
{
	class SceneManager
	{
	private:
		static std::string currentScene;
		static std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;
		static GLFWwindow* window;
	public:
		static void InitManager(GLFWwindow * windowIn)
		{
			window = windowIn;
		}
		static void SelectScene(const std::string& name)
		{
			if (Exists(currentScene))
			{
				scenes[currentScene]->StopScene();
			}
			if (!Exists(name))
			{
				currentScene = "";
				return;
			}

			currentScene = name;
			scenes[currentScene]->InitScene();
			
		}

		static bool Exists(const std::string& name)
		{
			return scenes.find(name) != scenes.end();
		}

		static void AddScene(const std::string& name)
		{
			if (Exists(name))
			{
				WARN("scene already exists");
				return;
			}

			scenes[name] = std::make_unique<Scene>(window);

		}

		static void RemoveScene(const std::string& name)
		{
			if (!Exists(name))
			{
				WARN("scene does not exist");
			}
			if (currentScene == name)
			{
				currentScene = "";
			}
			scenes.erase(name);

		}

		static void UpdateCurrentScene()
		{
			
		}

		static Scene* GetScene(const std::string& name)
		{
			if (!Exists(name))
			{
				return nullptr;
			}
				
			return scenes[name].get();
		}


	};
}
