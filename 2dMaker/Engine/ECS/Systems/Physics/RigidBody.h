#pragma once
#include "../Sysbase.h"
namespace D2Maker
{


	class RigidBodySystem : public System
	{
	private:
		std::vector<Entity> entities;
	public:
		void GetEntities(EntityManager& em);
		void Update(EntityManager& em);
	};

}