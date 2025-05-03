#pragma once
#include "../Sysbase.h"

namespace D2Maker
{
	class ColliderFunctions
	{
	public:
		static bool CheckCollision(const Rectangle& rec1, const Rectangle& rec2)
		{
			return 	rec1.x < rec2.x + rec2.width &&
				rec1.x + rec1.width > rec2.x &&
				rec1.y < rec2.y + rec2.height &&
				rec1.y + rec1.height > rec2.y;
		}

		static bool InViewport(Transform* transform)
		{
			Rectangle rec(transform->x, transform->y, transform->width, transform->height);
			Rectangle view(0, 0, VIRTUAL_WIDTH, VIRTUAL_HEIGHT);
			return CheckCollision(rec, view);
		}

		static bool CheckCollisionWithName(Entity entityIn,EntityManager &em,std::string nameIn)
		{
			float dt = DeltaTime::Get();
			if (!em.hasComponent<Collider>(entityIn))
			{
				return false;
			}
			if (!em.hasComponent<Velocity>(entityIn))
			{
				return false;
			}
			for (auto entity : em.aliveEntities)
			{
				if (entity == entityIn)
				{
					continue;
				}
				if (!em.hasComponent<Name>(entity))
				{
					continue;
				}
				if (!em.hasComponent<Collider>(entity))
				{
					continue;
				}
				Name* name = em.getComponent<Name>(entity);
				if (name->name != nameIn)
				{
					continue;
				}
				
				Transform* transform1 = em.getComponent<Transform>(entityIn);
				Velocity* vel = em.getComponent<Velocity>(entityIn);
				Transform* transform2 = em.getComponent<Transform>(entity);
				Rectangle staticRec{ transform2->x, transform2->y,
					transform2->width, transform2->height };
				Rectangle futureX{ transform1->x + vel->dx * dt, transform1->y,
					transform1->width, transform1->height };
				Rectangle futureY{ transform1->x, transform1->y + vel->dy * dt,
					transform1->width, transform1->height };

				if (CheckCollision(staticRec, futureX) || CheckCollision(staticRec, futureY))
				{
					return true;
				}
			}
			return false;
		}

		static void AdjustXStatic(Transform* transform, const Rectangle& rec, float dx)
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

		static void AdjustXDynamic(Transform* transform1, Transform* transform2, float dx1, float dx2)
		{
			float delta = (abs(transform1->x - transform2->x)) / 2.0f;
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
			else if (dx1 > 0 && dx2 < 0)
			{
				transform1->x += delta;
				transform2->x -= delta;

			}
		}

		static void AdjustYStatic(Transform* transform, const Rectangle& rec, float dy)
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

		static void AdjustYDynamic(Transform* transform1, Transform* transform2, float dy1, float dy2)
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
	};
}