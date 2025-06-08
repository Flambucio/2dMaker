#pragma once
#include "../Sysbase.h"
namespace D2Maker
{


	class CameraSystem : public System
	{
	public:
		void Update(EntityManager& em)
		{
			if (em.cameraEntity == 0)
			{
				if (em.aliveEntities.find(0) != em.aliveEntities.end() && !em.hasComponent<Camera>(0))
				{
					return;
				}
			}
			if (em.isVirtualEntity(em.cameraEntity)) { return; }

			Transform* cameraT = em.getComponent<Transform>(em.cameraEntity);
			Camera* cameraC = em.getComponent<Camera>(em.cameraEntity);
			int dx=0;
			int dy=0;
			if (!cameraC) return;
			if (!cameraT) return;
			if (cameraC->enableX)
			{
				dx = cameraC->x - cameraT->x;
			}
			if (cameraC->enableY)
			{
				dy = cameraC->y - cameraT->y;
			}

			for (Entity entity : em.aliveEntities)
			{
				if (!em.hasComponent<Transform>(entity))
				{
					continue;
				}
				Transform* transform = em.getComponent<Transform>(entity);
				transform->x += dx;
				transform->y += dy;
			}



			
		}
	};

}
