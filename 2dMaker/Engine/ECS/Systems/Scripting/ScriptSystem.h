#pragma once
#include "Interpreter.h"
#include "ASTBuilder.h"

namespace D2Maker
{
	class ScriptSystem : public System
	{
	public:
		inline void Update(EntityManager& em)
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

