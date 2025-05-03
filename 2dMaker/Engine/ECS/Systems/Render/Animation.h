#pragma once
#include "../Sysbase.h"

namespace D2Maker
{
	class AnimationSystem : public System
	{
	private:
		std::vector<Entity> animEntities;
	public:
		void Update(EntityManager&em)
		{
			IndexChanging(em);
			TextureAdding(em);
		}
		void IndexChanging(EntityManager&em)
		{
			animEntities.clear();
			for (Entity entity : em.aliveEntities)
			{
				if (em.isVirtualEntity(entity)) { continue; }
				if (!em.hasComponent<Animation>(entity)) { continue; }
				animEntities.push_back(entity);
				Animation* animation = em.getComponent<Animation>(entity);
				animation->accumulator += DeltaTime::Get();
				if (animation->accumulator >= animation->timing)
				{
					if (animation->index < animation->texNames.size() - 1)
					{
						animation->index++;
					}
					else
					{
						animation->index = 0;
					}
					animation->accumulator -= animation->timing;
				}
			}
		}
		void TextureAdding(EntityManager&em)
		{
			for (Entity entity : animEntities)
			{
				
				Animation* animation = em.getComponent < Animation>(entity);
				if (animation->index == animation->currentTextureIndex) { continue; }
				if (em.hasComponent<TextureComponent>(entity))
				{
					em.RemoveComponent<TextureComponent>(entity);
				}
				

				em.addComponent<TextureComponent>(entity, animation->texNames[animation->index],
					animation->orderInLayer);


			}
		}
	};
}