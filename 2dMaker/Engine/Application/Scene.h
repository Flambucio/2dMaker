#pragma once
#include "../ECS/SystemManager.h"
#include "../Events/EventManager.h"
namespace D2Maker
{
	class Scene
	{
	public:
		EntityManager em;
		SystemManager sm;
		void Update()
		{
			sm.UpdateSystems(em);
		}
		void InitScene()
		{
			AudioSystem & as= sm.GetAudioSystem();
			as.InitSounds(em);
			
		}
		void StopScene()
		{
			AudioSystem& as = sm.GetAudioSystem();
			as.StopSounds(em);
		}
	};
}