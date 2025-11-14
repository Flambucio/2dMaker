#pragma once
#include "../Sysbase.h"

namespace D2Maker
{
	class AnimationSystem : public System
	{
	private:
		std::vector<Entity> animEntities;
	public:
		inline void Update(EntityManager&em)
		{
			IndexChanging(em);
			TextureAdding(em);
		}
		void IndexChanging(EntityManager& em);
		void TextureAdding(EntityManager& em);
	};
}