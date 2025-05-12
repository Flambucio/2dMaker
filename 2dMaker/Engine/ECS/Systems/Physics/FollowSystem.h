#pragma once
#include "../Sysbase.h"

namespace D2Maker
{
	class FollowSystem : public System
	{
	public:
		void Update(EntityManager& em)
		{
			for (Entity entity : em.aliveEntities)
			{
				if (em.isVirtualEntity(entity))
				{
					continue;
				}

				if (!em.hasComponent<Follow>(entity)) continue;
				Follow* follow = em.getComponent<Follow>(entity);

				if (em.entityNames.find(follow->entityToFollow) == em.entityNames.end()) continue;

				Entity otherEntity = em.entityNames.find(follow->entityToFollow)->second;
				if (em.isVirtualEntity(otherEntity)) continue;
				if (!em.hasComponent<Transform>(otherEntity)) continue;

				Velocity* velocity = em.getComponent<Velocity>(entity);
				Transform* transform1 = em.getComponent<Transform>(entity);
				Transform* transform2 = em.getComponent<Transform>(otherEntity);
				

				if (em.hasComponent<RigidBody>(entity))
				{
					if (transform1->x > transform2->x)
					{
						velocity->dx -= follow->velocity;
					}
					else
					{
						velocity->dx += follow->velocity;
					}
					continue;
				}
				int deltaX = transform1->x - transform2->x;
				int deltaY = transform1->y - transform2->y;

				int hypothenuse = sqrt((pow(deltaX, 2) + pow(deltaY, 2)));

				if (hypothenuse != 0) 
				{
					float directionX = ((double)deltaX / (double)hypothenuse) * (double)follow->velocity;
					float directionY = ((double)deltaY / (double)hypothenuse) * (double)follow->velocity;

					velocity->dx -= (int)directionX;
					velocity->dy -= (int)directionY;
				}

				

				


			}
		}
	};
}
