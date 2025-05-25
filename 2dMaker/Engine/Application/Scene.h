#pragma once
#include "../ECS/SystemManager.h"
namespace D2Maker
{
	class Scene
	{
	public:
		EntityManager em;
		SystemManager sm;
		Scene(GLFWwindow* window) : sm(window)
		{}
		void InitScene()
		{
			AudioSystem & as= sm.GetAudioSystem();
			as.InitSounds(em);
			
		}
		void StopScene()
		{

			AudioSystem& as = sm.GetAudioSystem();
			as.StopSounds(em);
			for (Entity entity : em.aliveEntities)
			{
				if (!em.hasComponent<Transform>(entity)) continue;

				Transform * t = em.getComponent<Transform>(entity);
				t->ResetPos();
			}
		}
	};
}