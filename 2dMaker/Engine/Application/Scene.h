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
		inline void InitScene()
		{
			AudioSystem & as= sm.GetAudioSystem();
			as.InitSounds(em);
			
		}
		void StopScene();
	};
}