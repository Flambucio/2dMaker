#pragma once
#include "../Sysbase.h"

namespace D2Maker
{
	class FollowSystem : public System
	{
	public:
		void Update(EntityManager& em);
	};
}
