#include "Physics.h"
#include "FollowSystem.h"
#include "RigidBody.h"
using namespace D2Maker;


void Physics::CollideAll(EntityManager& em)
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

			if (ColliderFunctions::CheckCollision(futureX, staticRec))
			{
				ColliderFunctions::AdjustXStatic(transform, staticRec, velocity->dx);
				collidedX = true;
				velocity->dx = 0;
			}

			if (ColliderFunctions::CheckCollision(futureY, staticRec))
			{

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

			if (ColliderFunctions::CheckCollision(futureX, dynamicRec))
			{
				if (!em.hasComponent<Follow>(entity))
				{
					ColliderFunctions::AdjustXDynamic(transform, tr2, velocity->dx, vel2->dx);
				}

				collidedX = true;
			}

			if (ColliderFunctions::CheckCollision(futureY, dynamicRec))
			{
				if (!em.hasComponent<Follow>(entity))
				{
					ColliderFunctions::AdjustYDynamic(transform, tr2, velocity->dy, vel2->dy);
				}

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


//void CollideAll(EntityManager& em)

void Physics::SetToVector(EntityManager& em)
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

void FollowSystem::Update(EntityManager& em)
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

			velocity->dx -= (float)directionX;
			velocity->dy -= (float)directionY;
		}





	}
}

void RigidBodySystem::GetEntities(EntityManager& em)
{

	entities.clear();
	for (Entity entity : em.aliveEntities)
	{
		if (em.isVirtualEntity(entity)) { continue; }
		if (em.hasComponent<RigidBody>(entity))
		{
			entities.push_back(entity);
		}
	}
}


void RigidBodySystem::Update(EntityManager& em)
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