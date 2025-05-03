#pragma once
#include "Sysbase.h"

namespace D2Maker
{
	class AudioSystem
	{
		void InitSounds(EntityManager&em)
		{
			for (Entity entity : em.aliveEntities)
			{
				if (em.isVirtualEntity(entity)) { continue; }
				if (em.hasComponent<AudioComponent>(entity))
				{
					AudioComponent* audio = em.getComponent<AudioComponent>(entity);
					if (audio->exists)
					{
						unsigned int source = AudioLoader::GetSource(audio->name);
						alSourcei(source, AL_LOOPING, AL_TRUE);
						AudioLoader::PlayAudio(audio->name);
					}
				}
			}
		}

		void StopSounds(EntityManager&em)
		{
			for (Entity entity : em.aliveEntities)
			{
				if (em.hasComponent<AudioComponent>(entity))
				{
					AudioComponent* audio = em.getComponent<AudioComponent>(entity);
					if (audio->exists)
					{
						unsigned int source = AudioLoader::GetSource(audio->name);
						AudioLoader::StopAudio(audio->name);
					}
				}
			}
		}
	};
}
