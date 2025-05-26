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


			}
		}
	};
}