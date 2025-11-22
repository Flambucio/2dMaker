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

				//CONSOLELOG("ScriptSystem EM ptr = " + std::to_string((uint64_t)&em));
				Script * script = em.getComponent<Script>(entity);
				CONSOLELOG("Executing script for entity " + std::to_string(entity));
				CONSOLELOG("Statements count: " + std::to_string(script->statements.size()));
				//Interpreter::InterpretTokens(script->parsedStr, entity, em);
				for (auto& statement : script->statements)
				{
					statement->Execute();
				}
				
					


			}
		}
	};

}

