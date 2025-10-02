#pragma once
#include "../Sysbase.h"

namespace D2Maker
{
	class ColliderFunctions
	{
	public:
		static bool MousePointerCollisionWithRec(const Rectangle& rec);
		static bool CheckCollision(const Rectangle& rec1, const Rectangle& rec2);
		static bool InViewport(Transform* transform);
		static bool CheckCollisionWithName(Entity entityIn, EntityManager& em, std::string nameIn);
		static void AdjustXStatic(Transform* transform, const Rectangle& rec, float dx);
		static void AdjustXDynamic(Transform* transform1, Transform* transform2, float dx1, float dx2);
		static void AdjustYStatic(Transform* transform, const Rectangle& rec, float dy);
		static void AdjustYDynamic(Transform* transform1, Transform* transform2, float dy1, float dy2);
	};
}