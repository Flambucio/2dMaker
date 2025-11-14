#pragma once
#include "ColliderFunctions.h"


namespace D2Maker
{


	class Physics : public System
	{
	public:
		inline void Update(EntityManager& em)
		{
			SetToVector(em);
			CollideAll(em);
			
		}
	private:

		std::vector<Entity> staticEntities;
		std::vector<Entity> dynamicEntities;
		
		void CollideAll(EntityManager& em);
		void SetToVector(EntityManager& em);



		inline void ApplyMovementX(Transform* transform, Velocity* velocity)
		{
			transform->x += velocity->dx *DeltaTime::Get();
			velocity->dx = velocity->defaultDx;
		}

		inline void ApplyMovementY(Transform* transform, Velocity* velocity)
		{
			transform->y += velocity->dy * DeltaTime::Get();
			velocity->dy = velocity->defaultDy;
		}

		inline void ApplyRotation(Transform* transform, Velocity* velocity)
		{
			transform->rotationDegrees += velocity->dtheta * DeltaTime::Get();
			velocity->dtheta = velocity->defaultDtheta;
		}
	};

}