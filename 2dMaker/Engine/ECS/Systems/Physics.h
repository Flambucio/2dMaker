#pragma once
#include "Sysbase.h"


namespace D2Maker
{


	class Physics : public System
	{
	public:
		void Update(EntityManager& em)
		{
			SetToVector(em);
			RigidBodyUpdate(em);
			CollideAll(em);
			
		}
	private:

		std::vector<Entity> staticEntities;
		std::vector<Entity> dynamicEntities;

		void RigidBodyUpdate(EntityManager& em)
		{
			for (Entity entity : dynamicEntities)
			{
				if (em.hasComponent<RigidBody>(entity))
				{
					RigidBody* rigidbody = em.getComponent<RigidBody>(entity);
					Velocity* velocity = em.getComponent<Velocity>(entity);
					velocity->dy += rigidbody->mass + rigidbody->dy_accumulator;
					if (velocity->dy > rigidbody->max_dy)
					{
						velocity->dy = rigidbody->max_dy;
					}
					rigidbody->dy_accumulator = velocity->dy;
				}
			}


		}

		void CollideAll(EntityManager& em)
		{
			float dt = DeltaTime::Get();

			for (Entity entity : dynamicEntities)
			{
				Velocity* velocity = em.getComponent<Velocity>(entity);
				Transform* transform = em.getComponent<Transform>(entity);
				bool collidedX = false;
				bool collidedY = false;

				// --- Collide with static entities ---
				for (Entity staticE : staticEntities)
				{
					Transform* tr2 = em.getComponent<Transform>(staticE);
					if (!em.hasComponent<Collider>(staticE)) continue;

					Rectangle staticRec{ tr2->x, tr2->y, tr2->width, tr2->height };
					Rectangle futureX{ transform->x + velocity->dx * dt, transform->y, transform->width, transform->height };
					Rectangle futureY{ transform->x, transform->y + velocity->dy * dt, transform->width, transform->height };

					if (CheckCollision(futureX, staticRec)) {
						AdjustXStatic(transform, staticRec, velocity->dx);
						collidedX = true;
					}

					if (CheckCollision(futureY, staticRec)) {
						AdjustYStatic(transform, staticRec, velocity->dy);
						collidedY = true;

						if (em.hasComponent<RigidBody>(entity))
						{
							auto rb = em.getComponent<RigidBody>(entity);
							rb->dy_accumulator = -rb->dy_accumulator * rb->bounciness;
						}
					}
				}

				// --- Collide with other dynamic entities ---
				for (Entity other : dynamicEntities)
				{
					if (other == entity) continue;
					if (!em.hasComponent<Collider>(other)) continue;

					Transform* tr2 = em.getComponent<Transform>(other);
					Velocity* vel2 = em.getComponent<Velocity>(other);
					Rectangle dynamicRec{ tr2->x, tr2->y, tr2->width, tr2->height };
					Rectangle futureX{ transform->x + velocity->dx * dt, transform->y, transform->width, transform->height };
					Rectangle futureY{ transform->x, transform->y + velocity->dy * dt, transform->width, transform->height };

					if (CheckCollision(futureX, dynamicRec)) {
						AdjustXDynamic(transform, tr2, velocity->dx, vel2->dx);
						collidedX = true;
					}

					if (CheckCollision(futureY, dynamicRec)) {
						AdjustYDynamic(transform, tr2, velocity->dy, vel2->dy);
						collidedY = true;

						if (em.hasComponent<RigidBody>(entity))
						{
							auto rb = em.getComponent<RigidBody>(entity);
							rb->dy_accumulator = -rb->dy_accumulator * rb->bounciness;
						}
						if (em.hasComponent<RigidBody>(other))
						{
							auto rb = em.getComponent<RigidBody>(other);
							rb->dy_accumulator = -rb->dy_accumulator * rb->bounciness;
						}
					}
				}

				// Move only once
				if (!collidedX) ApplyMovementX(transform, velocity);
				if (!collidedY) ApplyMovementY(transform, velocity);
			}
		}


		bool CheckCollision(const Rectangle& rec1, const Rectangle& rec2)
		{
			return 	rec1.x < rec2.x + rec2.width &&
				rec1.x + rec1.width > rec2.x &&
				rec1.y < rec2.y + rec2.height &&
				rec1.y + rec1.height > rec2.y;
		}

		void AdjustXStatic(Transform* transform, const Rectangle& rec, float dx)
		{
			if (dx > 0)
			{
				transform->x = rec.x - transform->width;
			}
			else if (dx < 0)
			{
				transform->x = rec.x + rec.width;
			}
		}

		void AdjustXDynamic(Transform* transform1, Transform* transform2, float dx1, float dx2)
		{
			float delta = (abs(transform1->x - transform2->x))/2.0f;
			if (dx1 > 0 && dx2 > 0)
			{
				if (transform1->x < transform2->x)
				{
					transform1->x = transform2->x - transform1->width;
				}
				else if (transform2->x < transform1->x)
				{
					transform2->x = transform1->x - transform2->width;
				}
			}
			else if (dx1 < 0 && dx2 < 0)
			{
				if (transform1->x > transform2->x)
				{
					transform1->x = transform2->x + transform1->width;
				}
				else if (transform2->x > transform1->x)
				{
					transform2->x = transform1->x + transform2->width;
				}
			}
			else if (dx1 < 0 && dx2>0)
			{
				transform2->x += delta;
				transform1->x -= delta;
			}
			else if (dx1 > 0 && dx2<0)
			{
				transform1->x += delta;
				transform2->x -= delta;

			}
		}

		void AdjustYStatic(Transform* transform, const Rectangle& rec, float dy)
		{
			if (dy > 0)
			{
				transform->y = rec.y - transform->height;
			}
			else if (dy < 0)
			{
				transform->y = rec.y + rec.height;
			}
		}

		void AdjustYDynamic(Transform* transform1, Transform* transform2, float dy1, float dy2)
		{
			float delta = (abs(transform1->y - transform2->y)) / 2.0f;
			if (dy1 > 0 && dy2 > 0)
			{
				if (transform1->y < transform2->y)
				{
					transform1->y = transform2->y - transform1->height;
				}
				else if (transform2->y < transform1->y)
				{
					transform2->y = transform1->y - transform2->height;
				}
			}
			else if (dy1 < 0 && dy2 < 0)
			{
				if (transform1->y > transform2->y)
				{
					transform1->y = transform2->y + transform1->height;
				}
				else if (transform2->y > transform1->y)
				{
					transform2->y = transform1->y + transform2->height;
				}
			}
			else if (dy1 < 0 && dy2>0)
			{
				transform2->y += delta;
				transform1->y -= delta;
			}
			else if (dy1 > 0 && dy2 < 0)
			{
				transform1->y += delta;
				transform2->y -= delta;

			}
		}


		void SetToVector(EntityManager& em)
		{
			staticEntities.clear();
			dynamicEntities.clear();
			for (Entity entity : em.aliveEntities)
			{
				
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
			velocity->dx = velocity->default_dx;
		}

		void ApplyMovementY(Transform* transform, Velocity* velocity)
		{
			transform->y += velocity->dy * DeltaTime::Get();
			velocity->dy = velocity->default_dy;
		}
	};

}