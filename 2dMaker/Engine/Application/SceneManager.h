#pragma once

#include "Scene.h"

namespace D2Maker
{
	class SceneManager
	{
	private:
		std::string currentScene = "";
		std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;
	public:
		void SelectScene(const std::string& name)
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

		bool Exists(const std::string& name)
		{
			return scenes.find(name) != scenes.end();
		}

		void AddScene(const std::string& name)
		{
			if (Exists(name))
			{
				WARN("scene already exists");
				return;
			}

			scenes[name] = std::make_unique<Scene>();

		}

		void RemoveScene(const std::string& name)
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


	};
}
