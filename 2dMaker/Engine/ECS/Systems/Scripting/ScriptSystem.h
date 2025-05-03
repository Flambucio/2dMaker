#pragma once
#include "Interpreter.h"

namespace D2Maker
{
	class ScriptSystem : public System
	{
	public:
		void Update(EntityManager& em)
		{
			for (Entity entity : em.aliveEntities)
			{
				if (em.isVirtualEntity(entity)) { continue; }
				if (!em.hasComponent<Script>(entity))
				{
					continue;
				}

				Script * script = em.getComponent<Script>(entity);
				Interpreter::InterpretTokens(script->parsedStr, entity, em);
				
					


			}
		}
	};

}

