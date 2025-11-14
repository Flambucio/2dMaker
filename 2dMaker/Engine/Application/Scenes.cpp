#include "SceneManager.h"
using namespace D2Maker;

void Scene::StopScene()
{

	AudioSystem& as = sm.GetAudioSystem();
	as.StopSounds(em);
	for (Entity entity : em.aliveEntities)
	{
		if (em.hasComponent<Transform>(entity))
		{
			Transform* t = em.getComponent<Transform>(entity);
			t->ResetPos();
		}
		if (em.hasComponent<Animation>(entity))
		{
			Animation* a = em.getComponent<Animation>(entity);
			a->ResetIndexTimes();
			if (em.hasComponent<TextureComponent>(entity))
			{
				em.RemoveComponent<TextureComponent>(entity);
			}
			em.addComponent<TextureComponent>(entity, a->texNames[a->index],
				a->orderInLayer);
		}
		if (em.hasComponent<Timer>(entity))
		{
			Timer* timer = em.getComponent<Timer>(entity);
			timer->ResetAcc();
		}


	}
}




void SceneManager::SelectScene(const std::string& name)
{
	TRACE("EXISTS CURRENT:" + Exists(currentScene))
		if (Exists(currentScene))
		{
			TRACE("stopping scene");
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



bool SceneManager::AddScene(const std::string& name)
{
	if (Exists(name) || name == "")
	{
		WARN("scene already exists");
		return false;
	}

	scenes[name] = std::make_unique<Scene>(window);
	for (auto& element : scenes)
	{
		TRACE(element.first);
	}
	return true;

}

void SceneManager::RemoveScene(const std::string& name)
{
	if (!Exists(name))
	{
		WARN("scene does not exist");
	}
	if (currentScene == name)
	{
		currentScene = "";
	}
	Scene* currentScenePtr = GetScene(name);
	currentScenePtr->em.aliveEntities.clear();
	currentScenePtr->em.GetMap().clear();
	scenes.erase(name);

}

void SceneManager::UpdateCurrentScene(GLFWwindow* window, bool runGameFlag)
{
	if (currentScene != "")
	{
		if (window)
		{
			scenes[currentScene]->sm.UpdateSystems(scenes[currentScene]->em, runGameFlag);
		}
	}
}

Scene* SceneManager::GetScene(const std::string& name)
{
	if (!Exists(name))
	{
		return nullptr;
	}

	return scenes[name].get();
}
