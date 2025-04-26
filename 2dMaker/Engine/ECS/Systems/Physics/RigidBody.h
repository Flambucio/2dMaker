#pragma once
#include "../Sysbase.h"
namespace D2Maker
{


	class RigidBodySystem : public System
	{
	private:
		std::vector<Entity> entities;
	public:
		void GetEntities(EntityManager &em)
		{

			entities.clear();
			for (Entity entity : em.aliveEntities)
			{

				if (em.hasComponent<RigidBody>(entity))
				{
					entities.push_back(entity);
				}
			}
		}


		void Update(EntityManager&em)
		{
			GetEntities(em);
			for (Entity entity : entities)
			{
				RigidBody* rigidbody = em.getComponent<RigidBody>(entity);
				Velocity* velocity = em.getComponent<Velocity>(entity);
				velocity->dy += rigidbody->mass + rigidbody->dyAccumulator;
				if (velocity->dy > rigidbody->maxDy)
				{
					velocity->dy = rigidbody->maxDy;
				}
				rigidbody->dyAccumulator = velocity->dy;
			}
		}
	};

}