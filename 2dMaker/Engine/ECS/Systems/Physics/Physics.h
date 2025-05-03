#pragma once
#include "ColliderFunctions.h"


namespace D2Maker
{


	class Physics : public System
	{
	public:
		void Update(EntityManager& em)
		{
			SetToVector(em);
			CollideAll(em);
			
		}
	private:

		std::vector<Entity> staticEntities;
		std::vector<Entity> dynamicEntities;

		void CollideAll(EntityManager& em)
		{
			float dt = DeltaTime::Get();

			for (Entity entity : dynamicEntities)
			{
				Velocity* velocity = em.getComponent<Velocity>(entity);
				Transform* transform = em.getComponent<Transform>(entity);
				bool collidedX = false;
				bool collidedY = false;

				
				for (Entity staticE : staticEntities)
				{
					Transform* tr2 = em.getComponent<Transform>(staticE);
					if (!em.hasComponent<Collider>(staticE)) continue;

					Rectangle staticRec{ tr2->x, tr2->y,
						tr2->width, tr2->height };
					Rectangle futureX{ transform->x + velocity->dx * dt, transform->y,
						transform->width, transform->height };
					Rectangle futureY{ transform->x, transform->y + velocity->dy * dt, 
						transform->width, transform->height };

					if (ColliderFunctions::CheckCollision(futureX, staticRec)) {
						ColliderFunctions::AdjustXStatic(transform, staticRec, velocity->dx);
						collidedX = true;
						velocity->dx = 0;
					}

					if (ColliderFunctions::CheckCollision(futureY, staticRec)) {
						ColliderFunctions::AdjustYStatic(transform, staticRec, velocity->dy);
						collidedY = true;

						if (em.hasComponent<RigidBody>(entity))
						{
							auto rb = em.getComponent<RigidBody>(entity);

							rb->dyAccumulator = -rb->dyAccumulator * rb->bounciness;
						}
						velocity->dy = 0;
					}
				}

				
				for (Entity other : dynamicEntities)
				{
					if (other == entity)
					{
						continue;
					}
					if (!em.hasComponent<Collider>(other))
					{
						continue;
					}
					

					Transform* tr2 = em.getComponent<Transform>(other);
					Velocity* vel2 = em.getComponent<Velocity>(other);
					Rectangle dynamicRec{ tr2->x, tr2->y, tr2->width, tr2->height };
					Rectangle futureX{ transform->x + velocity->dx * dt, transform->y, transform->width, transform->height };
					Rectangle futureY{ transform->x, transform->y + velocity->dy * dt, transform->width, transform->height };

					if (ColliderFunctions::CheckCollision(futureX, dynamicRec)) {
						ColliderFunctions::AdjustXDynamic(transform, tr2, velocity->dx, vel2->dx);
						collidedX = true;
					}

					if (ColliderFunctions::CheckCollision(futureY, dynamicRec)) {
						ColliderFunctions::AdjustYDynamic(transform, tr2, velocity->dy, vel2->dy);
						collidedY = true;

						if (em.hasComponent<RigidBody>(entity))
						{
							auto rb = em.getComponent<RigidBody>(entity);
							rb->dyAccumulator = -rb->dyAccumulator * rb->bounciness;
						}
						if (em.hasComponent<RigidBody>(other))
						{
							auto rb = em.getComponent<RigidBody>(other);
							rb->dyAccumulator = -rb->dyAccumulator * rb->bounciness;
						}
					}
				}

				if (!collidedX)
				{
					ApplyMovementX(transform, velocity);
				}	
				if (!collidedY)
				{
					ApplyMovementY(transform, velocity);
				}
				ApplyRotation(transform, velocity);
					
					
			}
		}
		void SetToVector(EntityManager& em)
		{
			staticEntities.clear();
			dynamicEntities.clear();
			for (Entity entity : em.aliveEntities)
			{
				if (em.isVirtualEntity(entity)) { continue; }
				if (em.hasComponent<Velocity>(entity))
				{
					dynamicEntities.push_back(entity);
				}
				else
				{
					if (em.hasComponent<Collider>(entity))
					{
						staticEntities.push_back(entity);
					}
					
				}
			}

		}



		void ApplyMovementX(Transform* transform, Velocity* velocity)
		{
			transform->x += velocity->dx *DeltaTime::Get();
			velocity->dx = velocity->defaultDx;
		}

		void ApplyMovementY(Transform* transform, Velocity* velocity)
		{
			transform->y += velocity->dy * DeltaTime::Get();
			velocity->dy = velocity->defaultDy;
		}

		void ApplyRotation(Transform* transform, Velocity* velocity)
		{
			transform->rotationDegrees += velocity->dtheta * DeltaTime::Get();
			velocity->dtheta = velocity->defaultDtheta;
		}
	};

}